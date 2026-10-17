// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;
using CORE::PERCUSSION::strike;

constexpr Float ACCENTED = 0.75f;
constexpr Float SOFTEN = 0.6f;
constexpr Float FULL = 1.0f;

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

auto accented(const KIT::Kit &kit, Float velocity) -> Float {
  const Float hit = CORE::PERCUSSION::struck(velocity, kit.rows[KIT::CURVE]);
  if (hit >= ACCENTED) return hit;
  return hit * (FULL - kit.rows[KIT::ACCENT] * SOFTEN);
}

}  // namespace

void SOUND::PLUGINS::KIT::apply(Kit &kit, const AUDIO::PLUGIN::Event &event) {
  if (event.kind == AUDIO::PLUGIN::Event::CONTROLLER) {
    if (event.index >= PARAMETERS) return;
    kit.rows[event.index] =
      CORE::TABLE::clamped(SHEET, event.index, event.value);
    settle(kit, drummed(event.index));
    pan(kit, panned(event.index));
    return;
  }
  if (event.kind != AUDIO::PLUGIN::Event::NOTE_ON || event.value <= 0) return;
  const Pad *pad = found(event.index);
  if (pad == nullptr) return;
  ::STRIKES[pad->drum](kit, pad->part, ::accented(kit, event.value));
}
