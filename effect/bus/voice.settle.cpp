// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;
using CORE::DYNAMICS::Detector;

constexpr Float QUICK = 0.0005f;
constexpr Float SLOW = 0.025f;
constexpr Float LET = 0.05f;
constexpr Float LONG = 0.4f;
constexpr Float GRIP = 0.01f;
constexpr Float EASE = 0.3f;
constexpr Float KNEE = 6.0f;
constexpr Float PIVOT = 250.0f;
constexpr Float PUSH = 4.0f;
constexpr Float ONE = 1.0f;
constexpr Float HALF = 0.5f;

}  // namespace

void SOUND::PLUGINS::BUS::build(Bus &bus) {
  Detectors &detectors = bus.detectors;
  detectors.fast = Detector{.attack = ::QUICK, .release = ::LET};
  detectors.slow = Detector{.attack = ::SLOW, .release = ::LET};
  detectors.tail = Detector{.attack = ::QUICK, .release = ::LONG};
  detectors.glue = Detector{.attack = ::GRIP, .release = ::EASE};
  for (Detector *detector :
       {&detectors.fast, &detectors.slow, &detectors.tail, &detectors.glue})
    CORE::DYNAMICS::settle(*detector, bus.rate);
  bus.tilts.assign(bus.channels, {});
  bus.tapes.assign(bus.channels, {});
}

void SOUND::PLUGINS::BUS::settle(Bus &bus) {
  bus.computer.threshold = bus.rows[THRESHOLD];
  bus.computer.ratio = bus.rows[RATIO];
  bus.computer.knee = ::KNEE;
  CORE::DYNAMICS::settle(bus.computer);
  for (CORE::FILTER::Biquad &tilt : bus.tilts)
    CORE::FILTER::settle(
      tilt, CORE::FILTER::LOWSHELF, ::PIVOT, CORE::FILTER::FLAT, bus.rows[TILT],
      bus.rate);
  bus.push = ::ONE + bus.rows[DRIVE] * ::PUSH;
  bus.trim = CORE::DYNAMICS::gain(bus.rows[OUTPUT] - bus.rows[TILT] * ::HALF);
}

void SOUND::PLUGINS::BUS::apply(Bus &bus, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  bus.rows[event.index] = CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(bus);
}
