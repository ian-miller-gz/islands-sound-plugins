// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

auto voiced(const CHORUS::Effect &effect, CHORUS::Channel &strip, Float in)
  -> Float {
  Float wet = 0;
  for (Whole voice = 0; voice < effect.voices; ++voice)
    wet += CORE::LINE::read(
      strip.line, strip.sweeps[voice],
      CORE::MODULATOR::tick(strip.lfos[voice]));
  CORE::LINE::write(strip.line, in);
  return in * effect.dry + wet * effect.wet;
}

}  // namespace

void SOUND::CHORUS::render(
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
    for (Whole channel = 0; channel < effect.channels; ++channel)
      lanes[channel][frame] =
        ::voiced(effect, effect.strips[channel], lanes[channel][frame]);
    const Float heard = CORE::BLOCK::loudest(lanes, effect.channels, frame);
    if (heard > peak) peak = heard;
  }
  CORE::BLOCK::rest(cursor, events, count, steer);
  CORE::BLOCK::publish(effect.meter, peak);
}
