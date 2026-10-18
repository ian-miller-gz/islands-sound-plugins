// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;
using CORE::PERCUSSION::CLICK::RIM;
using CORE::PERCUSSION::HEIGHT::HIGH;
using CORE::PERCUSSION::HEIGHT::LOW;
using CORE::PERCUSSION::HEIGHT::MID;

constexpr Whole WHOLE = 0;

struct Pad {
  Whole pitch;
  Whole voice;
  Whole part;
};

constexpr Pad PADS[] = {
  {35, RHYTHM::KICK, WHOLE},
  {36, RHYTHM::KICK, WHOLE},
  {37, RHYTHM::RIM, RIM},
  {38, RHYTHM::SNARE, WHOLE},
  {39, RHYTHM::CLAP, WHOLE},
  {40, RHYTHM::SNARE, WHOLE},
  {41, RHYTHM::TOM, LOW},
  {42, RHYTHM::HAT, CORE::PERCUSSION::OPENING::CLOSED},
  {43, RHYTHM::TOM, LOW},
  {44, RHYTHM::HAT, CORE::PERCUSSION::OPENING::CLOSED},
  {45, RHYTHM::TOM, MID},
  {46, RHYTHM::HAT, CORE::PERCUSSION::OPENING::OPEN},
  {47, RHYTHM::TOM, MID},
  {48, RHYTHM::TOM, HIGH},
  {49, RHYTHM::CYMBAL, CORE::PERCUSSION::PLATE::CRASH},
  {50, RHYTHM::TOM, HIGH},
  {51, RHYTHM::CYMBAL, CORE::PERCUSSION::PLATE::RIDE},
  {56, RHYTHM::COWBELL, WHOLE},
  {57, RHYTHM::CYMBAL, CORE::PERCUSSION::PLATE::CRASH},
  {59, RHYTHM::CYMBAL, CORE::PERCUSSION::PLATE::RIDE}};

auto found(Whole pitch) -> const Pad * {
  for (const Pad &pad : PADS)
    if (pad.pitch == pitch) return &pad;
  return nullptr;
}

auto paced(Whole index) -> Flag {
  return index == RHYTHM::PATTERN || index == RHYTHM::TEMPO ||
         index == RHYTHM::SWING;
}

void run(RHYTHM::Box &box) {
  if (box.rows[RHYTHM::RUN] > 0) return CORE::CLOCK::run(box.clock);
  CORE::CLOCK::stop(box.clock);
  CORE::CLOCK::reset(box.clock);
  RHYTHM::rewind(box);
}

}  // namespace

void SOUND::PLUGINS::RHYTHM::apply(
  Box &box, const AUDIO::PLUGIN::Event &event) {
  if (event.kind == AUDIO::PLUGIN::Event::CONTROLLER) {
    if (event.index >= PARAMETERS) return;
    box.rows[event.index] =
      CORE::TABLE::clamped(SHEET, event.index, event.value);
    if (::paced(event.index)) pace(box);
    if (event.index == RUN) ::run(box);
    return;
  }
  if (event.kind != AUDIO::PLUGIN::Event::NOTE_ON || event.value <= 0) return;
  const Pad *pad = ::found(event.index);
  if (pad != nullptr) strike(box, pad->voice, pad->part, event.value);
}
