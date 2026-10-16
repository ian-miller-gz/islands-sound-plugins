// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float MIDDLE = 60;
constexpr Float HALF = 0.5f;
constexpr Float SEMITONE = CORE::PHASE::CENTS / CORE::PHASE::OCTAVE;

void hit(KICK::Voice &voice, Whole note, Float velocity) {
  if (voice.rows[KICK::KEY] < HALF) {
    CORE::PERCUSSION::strike(voice.kick, velocity);
    return;
  }
  const Float cents = (Float(note) - MIDDLE) * SEMITONE;
  const Float hertz = voice.rows[KICK::TUNE] * CORE::PHASE::ratio(cents);
  CORE::PERCUSSION::strike(voice.kick, hertz, velocity);
}

}  // namespace

void SOUND::PLUGINS::KICK::apply(
  Voice &voice, const AUDIO::PLUGIN::Event &event) {
  if (event.kind == AUDIO::PLUGIN::Event::CONTROLLER) {
    if (event.index >= PARAMETERS) return;
    voice.rows[event.index] =
      CORE::TABLE::clamped(SHEET, event.index, event.value);
    settle(voice);
    return;
  }
  if (event.kind == AUDIO::PLUGIN::Event::NOTE_ON && event.value > 0)
    ::hit(voice, event.index, event.value);
}
