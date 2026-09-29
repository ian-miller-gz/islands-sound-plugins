// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float MILLISECOND = 0.001f;
constexpr Float SPAN = 4;
constexpr Float CENTRE = 1.5f;
constexpr Float CROSSOVER = 800;
constexpr Float FULL = 1;
constexpr Float TWICE = 2;
constexpr CORE::PHASE::Wheel QUARTER = 0x40000000u;
constexpr Whole STOPPED = 0;

struct Cabinet {
  Float slow;
  Float fast;
  Float inertia;
  Float doppler;
  Float swing;
};

constexpr Cabinet HORN = {0.8f, 6.7f, 0.6f, 0.45f, 0.4f};
constexpr Cabinet DRUM = {0.67f, 5.7f, 2.5f, 0.2f, 0.25f};

void build(DRAWBAR::Rotor &rotor, const Cabinet &cabinet, Whole rate) {
  CORE::LINE::build(rotor.line, Whole(SPAN * MILLISECOND * Float(rate)));
  const Float doppler = cabinet.doppler * MILLISECOND;
  CORE::LINE::settle(rotor.near, CENTRE * MILLISECOND, doppler, 0, rate);
  CORE::LINE::settle(rotor.far, CENTRE * MILLISECOND, doppler, 0, rate);
  rotor.speed.time = cabinet.inertia;
  CORE::MODULATOR::settle(rotor.speed, rate);
  CORE::MODULATOR::jump(rotor.speed, cabinet.slow);
  rotor.swing = cabinet.swing;
}

auto spin(DRAWBAR::Rotor &rotor, const DRAWBAR::Organ &organ, Float in)
  -> DRAWBAR::Pair {
  const Float hertz = CORE::MODULATOR::tick(rotor.speed, rotor.target);
  rotor.phase += CORE::PHASE::step(hertz, organ.rate);
  const Float facing =
    CORE::OSCILLATOR::sine(organ.sine, rotor.phase + QUARTER);
  const Float left = CORE::LINE::read(rotor.line, rotor.near, -facing);
  const Float right = CORE::LINE::read(rotor.line, rotor.far, facing);
  CORE::LINE::write(rotor.line, in);
  return {
    left * (FULL + rotor.swing * facing),
    right * (FULL - rotor.swing * facing)};
}

}  // namespace

void SOUND::DRAWBAR::build(Rotary &rotary, Whole rate) {
  ::build(rotary.horn, ::HORN, rate);
  ::build(rotary.drum, ::DRUM, rate);
  CORE::FILTER::settle(
    rotary.low, CORE::FILTER::LOW, CROSSOVER, CORE::FILTER::FLAT, 0, rate);
  CORE::FILTER::settle(
    rotary.high, CORE::FILTER::HIGH, CROSSOVER, CORE::FILTER::FLAT, 0, rate);
}

void SOUND::DRAWBAR::settle(Rotary &rotary, Whole mode, Whole) {
  const Flag fast = mode > Whole(SLOW);
  rotary.spinning = mode != STOPPED;
  rotary.horn.target = fast ? ::HORN.fast : ::HORN.slow;
  rotary.drum.target = fast ? ::DRUM.fast : ::DRUM.slow;
}

auto SOUND::DRAWBAR::tick(Rotary &rotary, const Organ &organ, Float in)
  -> Pair {
  if (!rotary.spinning) return {in, in};
  const Pair horn =
    ::spin(rotary.horn, organ, CORE::FILTER::tick(rotary.high, in));
  const Pair drum =
    ::spin(rotary.drum, organ, CORE::FILTER::tick(rotary.low, in));
  const Float balance = organ.rows[BALANCE];
  const Float lows = TWICE * (FULL - balance);
  const Float highs = TWICE * balance;
  return {
    highs * horn.left + lows * drum.left,
    highs * horn.right + lows * drum.right};
}
