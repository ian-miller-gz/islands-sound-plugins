// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::PLUGINS::SNARE::settle(Voice &voice) {
  CORE::PERCUSSION::settle(
    voice.snare, voice.rows[TUNE], voice.rows[TONE], voice.rows[SNAPPY],
    voice.rows[DECAY], voice.rate);
}

void SOUND::PLUGINS::SNARE::apply(
  Voice &voice, const AUDIO::PLUGIN::Event &event) {
  if (event.kind == AUDIO::PLUGIN::Event::CONTROLLER) {
    if (event.index >= PARAMETERS) return;
    voice.rows[event.index] =
      CORE::TABLE::clamped(SHEET, event.index, event.value);
    settle(voice);
    return;
  }
  if (event.kind == AUDIO::PLUGIN::Event::NOTE_ON && event.value > 0)
    CORE::PERCUSSION::strike(voice.snare, event.value);
}
