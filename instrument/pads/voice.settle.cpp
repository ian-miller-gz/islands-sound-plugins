// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr CORE::PERCUSSION::Key KEYS[] = {
  {35, PADS::KICK}, {36, PADS::KICK}, {38, PADS::SNARE}, {40, PADS::SNARE},
  {50, PADS::HIGH}, {48, PADS::HIGH}, {47, PADS::MID},   {45, PADS::MID},
  {43, PADS::LOW},  {41, PADS::FLOOR}};

auto knob(const PADS::Kit &kit, Whole pad, Whole knob) -> Float {
  return kit.rows[PADS::place(pad, knob)];
}

}  // namespace

void SOUND::PLUGINS::PADS::settle(Kit &kit, Whole pad) {
  if (pad >= PADS) return;
  const CORE::PERCUSSION::Voicing voicing = {
    .tune = ::knob(kit, pad, TUNE),
    .bend = ::knob(kit, pad, BEND),
    .decay = ::knob(kit, pad, DECAY),
    .tone = ::knob(kit, pad, TONE),
    .noise = ::knob(kit, pad, NOISE),
    .click = ::knob(kit, pad, CLICK)};
  CORE::PERCUSSION::settle(kit.pads[pad], voicing, kit.rate);
}

void SOUND::PLUGINS::PADS::settle(Kit &kit) {
  for (Whole pad = 0; pad < PADS; ++pad) settle(kit, pad);
}

void SOUND::PLUGINS::PADS::apply(Kit &kit, const AUDIO::PLUGIN::Event &event) {
  if (event.kind == AUDIO::PLUGIN::Event::CONTROLLER) {
    if (event.index >= PARAMETERS) return;
    kit.rows[event.index] =
      CORE::TABLE::clamped(SHEET, event.index, event.value);
    if (event.index % KNOBS != LEVEL) settle(kit, event.index / KNOBS);
    return;
  }
  if (event.kind != AUDIO::PLUGIN::Event::NOTE_ON || event.value <= 0) return;
  const Whole pad = CORE::PERCUSSION::keyed(::KEYS, event.index, PADS);
  if (pad < PADS) CORE::PERCUSSION::strike(kit.pads[pad], event.value);
}
