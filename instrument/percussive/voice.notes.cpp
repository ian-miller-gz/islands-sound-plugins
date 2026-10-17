// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr CORE::PERCUSSION::Key KEYS[] = {
  {36, PERCUSSIVE::KICK},
  {38, PERCUSSIVE::SNARE},
  {42, PERCUSSIVE::CLOSED},
  {46, PERCUSSIVE::OPEN},
  {45, PERCUSSIVE::TOM}};

}  // namespace

void SOUND::PLUGINS::PERCUSSIVE::apply(
  Machine &machine, const AUDIO::PLUGIN::Event &event) {
  if (event.kind == AUDIO::PLUGIN::Event::CONTROLLER) {
    if (event.index >= PARAMETERS) return;
    machine.rows[event.index] =
      CORE::TABLE::clamped(SHEET, event.index, event.value);
    settle(machine, slotted(event.index));
    return;
  }
  if (event.kind != AUDIO::PLUGIN::Event::NOTE_ON || event.value <= 0) return;
  const Whole slot = CORE::PERCUSSION::keyed(::KEYS, event.index, SLOTS);
  if (slot >= SLOTS) return;
  const Float velocity =
    CORE::PERCUSSION::struck(event.value, machine.rows[CURVE]);
  choke(machine, slot);
  CORE::PERCUSSION::strike(machine.pads[slot], velocity);
}
