// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

auto flanged(
  const FLANGER::Effect &effect, FLANGER::Channel &strip, Float in,
  Float sweep) -> Float {
  const Float wet =
    CORE::LINE::tick(strip.line, strip.loop, in, strip.sweep, sweep);
  return (in + effect.sign * wet) * FLANGER::HALF;
}

}  // namespace

void SOUND::FLANGER::render(
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
    const Float sweep = CORE::MODULATOR::tick(effect.lfo);
    for (Whole channel = 0; channel < effect.channels; ++channel)
      lanes[channel][frame] =
        ::flanged(effect, effect.strips[channel], lanes[channel][frame], sweep);
    const Float heard = CORE::BLOCK::loudest(lanes, effect.channels, frame);
    if (heard > peak) peak = heard;
  }
  CORE::BLOCK::rest(cursor, events, count, steer);
  CORE::BLOCK::publish(effect.meter, peak);
}
