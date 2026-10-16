// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

auto SOUND::PLUGINS::PERCUSSIVE::slotted(Whole index) -> Whole {
  if (index < GAIN) return index % KNOBS == DECAY ? index / KNOBS : SLOTS;
  if (index >= TUNES && index < CURVE) return (index - TUNES) % SLOTS;
  return SLOTS;
}

void SOUND::PLUGINS::PERCUSSIVE::settle(Machine &machine, Whole slot) {
  if (slot >= SLOTS) return;
  const CORE::PERCUSSION::Voicing voicing = {
    .tune = machine.rows[TUNES + slot],
    .bend = machine.rows[BENDS + slot],
    .decay = machine.rows[place(slot, DECAY)],
    .tone = machine.rows[TONES + slot],
    .noise = machine.rows[NOISES + slot]};
  CORE::PERCUSSION::settle(machine.pads[slot], voicing, machine.rate);
}

void SOUND::PLUGINS::PERCUSSIVE::settle(Machine &machine) {
  for (Whole slot = 0; slot < SLOTS; ++slot) settle(machine, slot);
}
