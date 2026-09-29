// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;
using CORE::PERCUSSION::CLOSED;
using CORE::PERCUSSION::HIGH;
using CORE::PERCUSSION::LOW;
using CORE::PERCUSSION::MID;
using CORE::PERCUSSION::OPEN;

constexpr Whole WHOLE = 0;
constexpr Float FULL = 1.0f;
constexpr Float ACCENTED = 0.75f;
constexpr Float SOFTEN = 0.6f;

struct Pad {
  Whole pitch;
  Whole drum;
  Whole part;
};

constexpr Pad PADS[] = {{35, MACHINE::KICK, WHOLE},  {36, MACHINE::KICK, WHOLE},
                        {38, MACHINE::SNARE, WHOLE}, {39, MACHINE::CLAP, WHOLE},
                        {40, MACHINE::SNARE, WHOLE}, {41, MACHINE::TOM, LOW},
                        {42, MACHINE::HAT, CLOSED},  {43, MACHINE::TOM, LOW},
                        {44, MACHINE::HAT, CLOSED},  {45, MACHINE::TOM, MID},
                        {46, MACHINE::HAT, OPEN},    {47, MACHINE::TOM, MID},
                        {48, MACHINE::TOM, HIGH},    {50, MACHINE::TOM, HIGH}};

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
  }};

auto found(Whole pitch) -> const Pad * {
  for (const Pad &pad : PADS)
    if (pad.pitch == pitch) return &pad;
  return nullptr;
}

auto accented(const MACHINE::Machine &machine, Float velocity) -> Float {
  return velocity >= ACCENTED ? FULL
                              : FULL - machine.rows[MACHINE::ACCENT] * SOFTEN;
}

}  // namespace

void SOUND::MACHINE::apply(
  Machine &machine, const AUDIO::PLUGIN::Event &event) {
  if (event.kind == AUDIO::PLUGIN::Event::CONTROLLER) {
    if (event.index >= PARAMETERS) return;
    machine.rows[event.index] =
      CORE::TABLE::clamped(SHEET, event.index, event.value);
    if (event.index < ACCENT && event.index % KNOBS == TUNE)
      settle(machine, event.index / KNOBS);
    return;
  }
  if (event.kind != AUDIO::PLUGIN::Event::NOTE_ON || event.value <= 0) return;
  const Pad *pad = ::found(event.index);
  if (pad == nullptr) return;
  ::STRIKES[pad->drum](machine, pad->part, ::accented(machine, event.value));
}
