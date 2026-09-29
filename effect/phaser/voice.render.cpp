// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND;

void sweep(PHASER::Effect &effect) {
  const Float octaves = effect.swing * CORE::MODULATOR::tick(effect.lfo);
  const Float cutoff = effect.rows[PHASER::CENTRE] * std::exp2(octaves);
  CORE::FILTER::settle(effect.pole, CORE::FILTER::LOW, cutoff, effect.rate);
}

auto phased(const PHASER::Effect &effect, PHASER::Channel &strip, Float in)
  -> Float {
  Float passed = in + effect.rows[PHASER::FEEDBACK] * strip.fed;
  for (Whole stage = 0; stage < effect.stages; ++stage) {
    CORE::FILTER::Pole &pole = strip.stages[stage];
    pole.gain = effect.pole.gain;
    passed = PHASER::DOUBLE * CORE::FILTER::tick(pole, passed) - passed;
  }
  strip.fed = passed;
  return (in + passed) * PHASER::HALF;
}

}  // namespace

void SOUND::PHASER::render(
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
    ::sweep(effect);
    for (Whole channel = 0; channel < effect.channels; ++channel)
      lanes[channel][frame] =
        ::phased(effect, effect.strips[channel], lanes[channel][frame]);
    const Float heard = CORE::BLOCK::loudest(lanes, effect.channels, frame);
    if (heard > peak) peak = heard;
  }
  CORE::BLOCK::rest(cursor, events, count, steer);
  CORE::BLOCK::publish(effect.meter, peak);
}
