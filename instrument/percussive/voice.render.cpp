// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float FULL = 1.0f;

auto faded(PERCUSSIVE::Machine &machine, Whole slot) -> Float {
  return slot == PERCUSSIVE::OPEN ? CORE::PERCUSSION::tick(machine.choke)
                                  : FULL;
}

}  // namespace

auto SOUND::PLUGINS::PERCUSSIVE::sound(Machine &machine) -> Float {
  Float sum = 0;
  for (Whole slot = 0; slot < SLOTS; ++slot)
    sum += CORE::PERCUSSION::tick(machine.pads[slot]) * ::faded(machine, slot) *
           machine.rows[place(slot, LEVEL)];
  return sum * machine.rows[GAIN];
}

void SOUND::PLUGINS::PERCUSSIVE::render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &machine = *static_cast<Machine *>(instance);
  const auto voiced = [&machine] { return sound(machine); };
  const auto take = [&machine](const AUDIO::PLUGIN::Event &event) {
    apply(machine, event);
  };
  const Float peak = CORE::PERCUSSION::play(
    lanes, machine.channels, frames, events, count, voiced, take);
  CORE::BLOCK::publish(machine.meter, peak);
}
