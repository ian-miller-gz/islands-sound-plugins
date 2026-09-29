// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;
using CORE::PERCUSSION::CLAVE;
using CORE::PERCUSSION::CLOSED;
using CORE::PERCUSSION::CRASH;
using CORE::PERCUSSION::HIGH;
using CORE::PERCUSSION::LOW;
using CORE::PERCUSSION::MID;
using CORE::PERCUSSION::OPEN;
using CORE::PERCUSSION::RIDE;
using CORE::PERCUSSION::RIM;
using CORE::PERCUSSION::strike;

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
  {35, KIT::KICK, WHOLE},  {36, KIT::KICK, WHOLE},   {37, KIT::CLAVE, RIM},
  {38, KIT::SNARE, WHOLE}, {39, KIT::CLAP, WHOLE},   {40, KIT::SNARE, WHOLE},
  {41, KIT::TOM, LOW},     {42, KIT::HAT, CLOSED},   {43, KIT::TOM, LOW},
  {44, KIT::HAT, CLOSED},  {45, KIT::TOM, MID},      {46, KIT::HAT, OPEN},
  {47, KIT::TOM, MID},     {48, KIT::TOM, HIGH},     {49, KIT::CYMBAL, CRASH},
  {50, KIT::TOM, HIGH},    {51, KIT::CYMBAL, RIDE},  {52, KIT::CYMBAL, CRASH},
  {53, KIT::CYMBAL, RIDE}, {55, KIT::CYMBAL, CRASH}, {56, KIT::COWBELL, WHOLE},
  {62, KIT::CONGA, HIGH},  {63, KIT::CONGA, MID},    {64, KIT::CONGA, LOW},
  {75, KIT::CLAVE, CLAVE}};

using Strike = void (*)(KIT::Kit &kit, Whole part, Float velocity);

constexpr Strike STRIKES[KIT::DRUMS] = {
  [](KIT::Kit &kit, Whole, Float velocity) { strike(kit.kick, velocity); },
  [](KIT::Kit &kit, Whole, Float velocity) { strike(kit.snare, velocity); },
  [](KIT::Kit &kit, Whole part, Float velocity) {
    strike(kit.hat, part, velocity);
  },
  [](KIT::Kit &kit, Whole, Float velocity) { strike(kit.clap, velocity); },
  [](KIT::Kit &kit, Whole part, Float velocity) {
    strike(kit.tom, part, velocity);
  },
  [](KIT::Kit &kit, Whole part, Float velocity) {
    strike(kit.cymbal, part, velocity);
  },
  [](KIT::Kit &kit, Whole, Float velocity) { strike(kit.cowbell, velocity); },
  [](KIT::Kit &kit, Whole part, Float velocity) {
    strike(kit.clave, part, velocity);
  },
  [](KIT::Kit &kit, Whole part, Float velocity) {
    strike(kit.conga, part, velocity);
  }};

auto found(Whole pitch) -> const Pad * {
  for (const Pad &pad : PADS)
    if (pad.pitch == pitch) return &pad;
  return nullptr;
}

auto accented(const KIT::Kit &kit, Float velocity) -> Float {
  return velocity >= ACCENTED ? FULL : FULL - kit.rows[KIT::ACCENT] * SOFTEN;
}

}  // namespace

void SOUND::KIT::apply(Kit &kit, const AUDIO::PLUGIN::Event &event) {
  if (event.kind == AUDIO::PLUGIN::Event::CONTROLLER) {
    if (event.index >= PARAMETERS) return;
    kit.rows[event.index] =
      CORE::TABLE::clamped(SHEET, event.index, event.value);
    if (event.index < ACCENT && event.index % KNOBS == TUNE)
      settle(kit, event.index / KNOBS);
    return;
  }
  if (event.kind != AUDIO::PLUGIN::Event::NOTE_ON || event.value <= 0) return;
  const Pad *pad = ::found(event.index);
  if (pad == nullptr) return;
  ::STRIKES[pad->drum](kit, pad->part, ::accented(kit, event.value));
}
