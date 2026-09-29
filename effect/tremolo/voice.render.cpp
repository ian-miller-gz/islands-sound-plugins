// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

auto swayed(const TREMOLO::Effect &effect, TREMOLO::Channel &strip) -> Float {
  const Float wave = CORE::MODULATOR::tick(strip.lfo);
  const Float dip = effect.rows[TREMOLO::DEPTH] * (TREMOLO::UNITY - wave);
  return CORE::MODULATOR::tick(
    strip.smoother, TREMOLO::UNITY - dip * TREMOLO::HALF);
}

}  // namespace

void SOUND::TREMOLO::render(
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
      lanes[channel][frame] *= ::swayed(effect, effect.strips[channel]);
    const Float heard = CORE::BLOCK::loudest(lanes, effect.channels, frame);
    if (heard > peak) peak = heard;
  }
  CORE::BLOCK::rest(cursor, events, count, steer);
  CORE::BLOCK::publish(effect.meter, peak);
}
