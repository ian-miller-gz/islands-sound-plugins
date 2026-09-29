// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;
using CORE::PERCUSSION::strike;

constexpr Whole WHOLE = 0;

struct Pad {
  Whole pitch;
  Whole voice;
  Whole part;
};

constexpr Pad PADS[] = {
  {35, RHYTHM::KICK, WHOLE},
  {36, RHYTHM::KICK, WHOLE},
  {38, RHYTHM::SNARE, WHOLE},
  {40, RHYTHM::SNARE, WHOLE},
  {42, RHYTHM::HAT, CORE::PERCUSSION::OPENING::CLOSED},
  {44, RHYTHM::HAT, CORE::PERCUSSION::OPENING::CLOSED},
  {46, RHYTHM::HAT, CORE::PERCUSSION::OPENING::OPEN},
  {49, RHYTHM::CYMBAL, CORE::PERCUSSION::PLATE::CRASH},
  {51, RHYTHM::CYMBAL, CORE::PERCUSSION::PLATE::RIDE},
  {57, RHYTHM::CYMBAL, CORE::PERCUSSION::PLATE::CRASH},
  {59, RHYTHM::CYMBAL, CORE::PERCUSSION::PLATE::RIDE}};

using Strike = void (*)(RHYTHM::Box &box, Whole part, Float velocity);

constexpr Strike STRIKES[RHYTHM::VOICES] = {
  [](RHYTHM::Box &box, Whole, Float velocity) { strike(box.kick, velocity); },
  [](RHYTHM::Box &box, Whole, Float velocity) { strike(box.snare, velocity); },
  [](RHYTHM::Box &box, Whole part, Float velocity) {
    strike(box.hat, part, velocity);
  },
  [](RHYTHM::Box &box, Whole part, Float velocity) {
    strike(box.cymbal, part, velocity);
  }};

auto found(Whole pitch) -> const Pad * {
  for (const Pad &pad : PADS)
    if (pad.pitch == pitch) return &pad;
  return nullptr;
}

void run(RHYTHM::Box &box) {
  if (box.rows[RHYTHM::RUN] > 0) return CORE::CLOCK::run(box.clock);
  CORE::CLOCK::stop(box.clock);
  CORE::CLOCK::reset(box.clock);
}

}  // namespace

void SOUND::PLUGINS::RHYTHM::strike(
  Box &box, Whole voice, Whole part, Float velocity) {
  if (voice < VOICES) ::STRIKES[voice](box, part, velocity);
}

void SOUND::PLUGINS::RHYTHM::apply(
  Box &box, const AUDIO::PLUGIN::Event &event) {
  if (event.kind == AUDIO::PLUGIN::Event::CONTROLLER) {
    if (event.index >= PARAMETERS) return;
    box.rows[event.index] =
      CORE::TABLE::clamped(SHEET, event.index, event.value);
    if (event.index == PATTERN || event.index == TEMPO) pace(box);
    if (event.index == RUN) ::run(box);
    return;
  }
  if (event.kind != AUDIO::PLUGIN::Event::NOTE_ON || event.value <= 0) return;
  const Pad *pad = ::found(event.index);
  if (pad != nullptr) strike(box, pad->voice, pad->part, event.value);
}
