// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

using Tick = auto (*)(MACHINE::Machine &machine) -> Float;

constexpr Tick TICKS[MACHINE::DRUMS] = {
  [](MACHINE::Machine &machine) -> Float {
    return CORE::PERCUSSION::HYBRID::tick(machine.kick);
  },
  [](MACHINE::Machine &machine) -> Float {
    return CORE::PERCUSSION::HYBRID::tick(machine.snare);
  },
  [](MACHINE::Machine &machine) -> Float {
    return CORE::PERCUSSION::HYBRID::tick(machine.hat);
  },
  [](MACHINE::Machine &machine) -> Float {
    return CORE::PERCUSSION::tick(machine.clap);
  },
  [](MACHINE::Machine &machine) -> Float {
    return CORE::PERCUSSION::HYBRID::tick(machine.tom);
  }};

}  // namespace

auto SOUND::MACHINE::mix(Machine &machine) -> Float {
  Float sum = 0;
  for (Whole drum = 0; drum < DRUMS; ++drum)
    sum += ::TICKS[drum](machine) * machine.rows[place(drum, LEVEL)];
  return CORE::SHAPER::soft(sum);
}

void SOUND::MACHINE::render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &machine = *static_cast<Machine *>(instance);
  const auto sound = [&machine] { return mix(machine); };
  const auto take = [&machine](const AUDIO::PLUGIN::Event &event) {
    apply(machine, event);
  };
  const Float peak = CORE::PERCUSSION::play(
    lanes, machine.channels, frames, events, count, sound, take);
  CORE::BLOCK::publish(machine.meter, peak);
}
