// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float HALF = 0.5f;

constexpr CORE::PERCUSSION::Key KEYS[] = {
  {51, CORE::PERCUSSION::PLATE::RIDE},
  {53, CORE::PERCUSSION::PLATE::BELL},
  {59, CORE::PERCUSSION::PLATE::RIDE}};

auto lifted(const AUDIO::PLUGIN::Event &event) -> Flag {
  return event.kind == AUDIO::PLUGIN::Event::NOTE_OFF ||
         (event.kind == AUDIO::PLUGIN::Event::NOTE_ON && event.value <= 0);
}

void lift(CYMBAL::Voice &voice, Whole note) {
  if (voice.rows[CYMBAL::CHOKE] > HALF && note == voice.note)
    CORE::PERCUSSION::lift(voice.cymbal);
}

void strike(CYMBAL::Voice &voice, Whole note, Float velocity) {
  const Whole plate =
    CORE::PERCUSSION::keyed(KEYS, note, CORE::PERCUSSION::PLATE::CRASH);
  voice.note = note;
  CORE::PERCUSSION::strike(voice.cymbal, plate, velocity);
}

}  // namespace

void SOUND::PLUGINS::CYMBAL::apply(
  Voice &voice, const AUDIO::PLUGIN::Event &event) {
  if (event.kind == AUDIO::PLUGIN::Event::CONTROLLER) {
    if (event.index >= PARAMETERS) return;
    voice.rows[event.index] =
      CORE::TABLE::clamped(SHEET, event.index, event.value);
    settle(voice);
    return;
  }
  if (::lifted(event)) {
    ::lift(voice, event.index);
    return;
  }
  if (event.kind == AUDIO::PLUGIN::Event::NOTE_ON)
    ::strike(voice, event.index, event.value);
}
