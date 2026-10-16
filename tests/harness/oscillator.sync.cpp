// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>
#include <cstdio>

#include "../../core/oscillator/oscillator.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole RATE = 48000;
constexpr Whole FRAMES = 4800;
constexpr Float SLAVE = 330;
constexpr Float MASTER = 110;
constexpr Float WIDTH = 0.3f;
constexpr Float SINCE = 0.25f;

void seat(CORE::OSCILLATOR::Oscillator &oscillator, Float hertz) {
  CORE::OSCILLATOR::settle(oscillator, hertz, WIDTH, RATE);
}

auto plain(Whole wave) -> Float {
  CORE::OSCILLATOR::Oscillator oscillator;
  CORE::OSCILLATOR::Sync sync;
  ::seat(oscillator, SLAVE);
  ::seat(sync.slave, SLAVE);
  Float most = 0;
  for (Whole frame = 0; frame < FRAMES; ++frame) {
    const Float alone = CORE::OSCILLATOR::tick(oscillator, wave);
    const Float none =
      CORE::OSCILLATOR::tick(sync, CORE::OSCILLATOR::NONE, wave);
    most = std::fmax(most, std::fabs(alone - none));
  }
  return most;
}

auto driven(Whole wave) -> Float {
  CORE::OSCILLATOR::Oscillator master;
  CORE::OSCILLATOR::Sync standing, edged;
  ::seat(master, MASTER);
  ::seat(standing.slave, SLAVE);
  ::seat(edged.slave, SLAVE);
  Float most = 0;
  for (Whole frame = 0; frame < FRAMES; ++frame) {
    master.phase += master.step;
    const Flag wrapped = master.phase < master.step;
    const Float since = Float(master.phase) / Float(master.step);
    const Float by = CORE::OSCILLATOR::tick(standing, master, wave);
    const Float at = CORE::OSCILLATOR::tick(
      edged, wrapped ? since : CORE::OSCILLATOR::NONE, wave);
    most = std::fmax(most, std::fabs(by - at));
  }
  return most;
}

auto placed(Whole wave) -> Float {
  CORE::OSCILLATOR::Sync sync;
  ::seat(sync.slave, SLAVE);
  CORE::OSCILLATOR::tick(sync, SINCE, wave);
  return Float(sync.slave.phase) / (Float(sync.slave.step) * SINCE);
}

void print(STRING::Hot word, auto (*measure)(Whole)->Float) {
  std::printf("%s", word);
  for (Whole wave = 0; wave < CORE::OSCILLATOR::WAVES; ++wave)
    std::printf(" %.9g", static_cast<double>(measure(wave)));
  std::printf("\n");
}

}  // namespace

namespace SOUND::PLUGINS::HARNESS::OSCILLATOR {

void synced() {
  ::print("plain", ::plain);
  ::print("driven", ::driven);
  ::print("placed", ::placed);
}

}  // namespace SOUND::PLUGINS::HARNESS::OSCILLATOR
