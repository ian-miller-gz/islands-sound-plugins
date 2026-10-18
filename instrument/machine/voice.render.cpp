// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

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
  },
  [](MACHINE::Machine &machine) -> Float {
    return CORE::PERCUSSION::HYBRID::tick(machine.cymbal);
  },
  [](MACHINE::Machine &machine) -> Float {
    return CORE::PERCUSSION::tick(machine.rim);
  }};

}  // namespace

auto SOUND::PLUGINS::MACHINE::mix(Machine &machine) -> CORE::STEREO::Pair {
  CORE::STEREO::Pair sum;
  for (Whole drum = 0; drum < DRUMS; ++drum) {
    const Float sample =
      ::TICKS[drum](machine) * machine.rows[SEATS[drum].level];
    CORE::STEREO::add(sum, sample, machine.gains[drum]);
  }
  return {CORE::SHAPER::soft(sum.left), CORE::SHAPER::soft(sum.right)};
}

void SOUND::PLUGINS::MACHINE::render(
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
