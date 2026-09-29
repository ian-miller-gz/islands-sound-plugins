// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

auto heard(
  ROTARY::Effect &effect, AUDIO::PLUGIN::Sample *const *lanes,
  Whole frame) -> Float {
  Float sum = 0;
  for (Whole channel = 0; channel < effect.channels; ++channel)
    sum += lanes[channel][frame];
  return sum / Float(effect.channels);
}

auto split(CORE::FILTER::Biquad *stages, Float in) -> Float {
  for (Whole stage = 0; stage < ROTARY::ORDER; ++stage)
    in = CORE::FILTER::tick(stages[stage], in);
  return in;
}

void spin(ROTARY::Rotor &rotor, Float in, Whole rate, Float *sides) {
  const Float speed = CORE::MODULATOR::tick(rotor.speed, rotor.target);
  const CORE::PHASE::Wheel step = CORE::PHASE::step(speed, rate);
  for (Whole side = 0; side < ROTARY::SIDES; ++side) {
    rotor.lfos[side].step = step;
    const Float wave = CORE::MODULATOR::tick(rotor.lfos[side]);
    const Float voiced =
      CORE::LINE::read(rotor.line, rotor.sweeps[side], -wave);
    const Float shade = ROTARY::UNITY + rotor.depth * (wave - ROTARY::UNITY);
    sides[side] += voiced * shade;
  }
  CORE::LINE::write(rotor.line, in);
}

void place(
  const ROTARY::Effect &effect, AUDIO::PLUGIN::Sample *const *lanes,
  Whole frame, const Float *sides) {
  const Float middle =
    (sides[ROTARY::LEFT] + sides[ROTARY::RIGHT]) * ROTARY::HALF;
  for (Whole channel = 0; channel < effect.channels; ++channel)
    lanes[channel][frame] =
      effect.channels == ROTARY::MONO ? middle : sides[channel % ROTARY::SIDES];
}

}  // namespace

void SOUND::ROTARY::render(
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
    const Float in = ::heard(effect, lanes, frame);
    const Float bands[ROTORS] = {
      ::split(effect.highs, in), ::split(effect.lows, in)};
    Float sides[SIDES] = {0, 0};
    for (Whole at = 0; at < ROTORS; ++at)
      ::spin(effect.rotors[at], bands[at], effect.rate, sides);
    ::place(effect, lanes, frame, sides);
    const Float loud = CORE::BLOCK::loudest(lanes, effect.channels, frame);
    if (loud > peak) peak = loud;
  }
  CORE::BLOCK::rest(cursor, events, count, steer);
  CORE::BLOCK::publish(effect.meter, peak);
}
