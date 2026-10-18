// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;
using CORE::PERCUSSION::CLICK::RIM;
using CORE::PERCUSSION::HEIGHT::HIGH;
using CORE::PERCUSSION::HEIGHT::LOW;
using CORE::PERCUSSION::HEIGHT::MID;
using CORE::PERCUSSION::HYBRID::STROKE::CRASH;
using CORE::PERCUSSION::HYBRID::STROKE::RIDE;
using CORE::PERCUSSION::OPENING::CLOSED;
using CORE::PERCUSSION::OPENING::OPEN;

constexpr Whole WHOLE = 0;
constexpr Float FULL = 1.0f;
constexpr Float ACCENTED = 0.75f;
constexpr Float SOFTEN = 0.6f;

struct Pad {
  Whole pitch;
  Whole drum;
  Whole part;
};

constexpr Pad PADS[] = {
  {35, MACHINE::KICK, WHOLE},   {36, MACHINE::KICK, WHOLE},
  {37, MACHINE::RIM, RIM},      {38, MACHINE::SNARE, WHOLE},
  {39, MACHINE::CLAP, WHOLE},   {40, MACHINE::SNARE, WHOLE},
  {41, MACHINE::TOM, LOW},      {42, MACHINE::HAT, CLOSED},
  {43, MACHINE::TOM, LOW},      {44, MACHINE::HAT, CLOSED},
  {45, MACHINE::TOM, MID},      {46, MACHINE::HAT, OPEN},
  {47, MACHINE::TOM, MID},      {48, MACHINE::TOM, HIGH},
  {49, MACHINE::CYMBAL, CRASH}, {50, MACHINE::TOM, HIGH},
  {51, MACHINE::CYMBAL, RIDE},  {52, MACHINE::CYMBAL, CRASH},
  {53, MACHINE::CYMBAL, RIDE},  {55, MACHINE::CYMBAL, CRASH},
  {57, MACHINE::CYMBAL, CRASH}, {59, MACHINE::CYMBAL, RIDE}};

using Strike = void (*)(MACHINE::Machine &machine, Whole part, Float velocity);

constexpr Strike STRIKES[MACHINE::DRUMS] = {
  [](MACHINE::Machine &machine, Whole, Float velocity) {
    CORE::PERCUSSION::HYBRID::strike(machine.kick, velocity);
  },
  [](MACHINE::Machine &machine, Whole, Float velocity) {
    CORE::PERCUSSION::HYBRID::strike(machine.snare, velocity);
  },
  [](MACHINE::Machine &machine, Whole part, Float velocity) {
    CORE::PERCUSSION::HYBRID::strike(machine.hat, part, velocity);
  },
  [](MACHINE::Machine &machine, Whole, Float velocity) {
    CORE::PERCUSSION::strike(machine.clap, velocity);
  },
  [](MACHINE::Machine &machine, Whole part, Float velocity) {
    CORE::PERCUSSION::HYBRID::strike(machine.tom, part, velocity);
  },
  [](MACHINE::Machine &machine, Whole part, Float velocity) {
    CORE::PERCUSSION::HYBRID::strike(machine.cymbal, part, velocity);
  },
  [](MACHINE::Machine &machine, Whole part, Float velocity) {
    CORE::PERCUSSION::strike(machine.rim, part, velocity);
  }};

auto found(Whole pitch) -> const Pad * {
  for (const Pad &pad : PADS)
    if (pad.pitch == pitch) return &pad;
  return nullptr;
}

auto accented(const MACHINE::Machine &machine, Float velocity) -> Float {
  const Float curved =
    CORE::PERCUSSION::struck(velocity, machine.rows[MACHINE::CURVE]);
  const Float accent = machine.rows[MACHINE::ACCENT];
  return velocity >= ACCENTED ? curved : curved * (FULL - accent * SOFTEN);
}

}  // namespace

void SOUND::PLUGINS::MACHINE::apply(
  Machine &machine, const AUDIO::PLUGIN::Event &event) {
  if (event.kind == AUDIO::PLUGIN::Event::CONTROLLER) {
    if (event.index >= PARAMETERS) return;
    machine.rows[event.index] =
      CORE::TABLE::clamped(SHEET, event.index, event.value);
    settle(machine, owner(event.index));
    return;
  }
  if (event.kind != AUDIO::PLUGIN::Event::NOTE_ON || event.value <= 0) return;
  const Pad *pad = ::found(event.index);
  if (pad == nullptr) return;
  ::STRIKES[pad->drum](machine, pad->part, ::accented(machine, event.value));
}
