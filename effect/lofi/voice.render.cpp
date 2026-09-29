// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto worn(LOFI::Effect &effect, LOFI::Strip &strip, Float in, Float wave)
  -> Float {
  const Float wobbled = CORE::LINE::read(strip.line, strip.sweep, wave);
  CORE::LINE::write(strip.line, in);
  const Float narrowed = CORE::FILTER::tick(strip.before, wobbled);
  const Float crushed = CORE::SHAPER::tick(strip.crusher, narrowed);
  const Float hiss = effect.floor * CORE::NOISE::tick(strip.white);
  return CORE::FILTER::tick(strip.after, crushed) + hiss;
}

}  // namespace

void SOUND::PLUGINS::LOFI::render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &effect = *static_cast<Effect *>(instance);
  const auto steer = [&effect](const AUDIO::PLUGIN::Event &event) {
    apply(effect, event);
  };
  CORE::BLOCK::Cursor cursor;
  Float peak = 0;
  for (Whole frame = 0; frame < frames; ++frame) {
    CORE::BLOCK::due(cursor, events, count, frame, steer);
    const Float wave = CORE::MODULATOR::tick(effect.lfo);
    for (Whole channel = 0; channel < effect.channels; ++channel)
      lanes[channel][frame] =
        ::worn(effect, effect.strips[channel], lanes[channel][frame], wave);
    const Float loud = CORE::BLOCK::loudest(lanes, effect.channels, frame);
    if (loud > peak) peak = loud;
  }
  CORE::BLOCK::rest(cursor, events, count, steer);
  CORE::BLOCK::publish(effect.meter, peak);
}
