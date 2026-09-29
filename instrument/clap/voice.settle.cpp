// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::CLAP::settle(Voice &voice) {
  CORE::PERCUSSION::settle(
    voice.clap, voice.rows[SPREAD], voice.rows[DECAY], voice.rows[TONE],
    voice.rate);
}

void SOUND::CLAP::apply(Voice &voice, const AUDIO::PLUGIN::Event &event) {
  if (event.kind == AUDIO::PLUGIN::Event::CONTROLLER) {
    if (event.index >= PARAMETERS) return;
    voice.rows[event.index] =
      CORE::TABLE::clamped(SHEET, event.index, event.value);
    settle(voice);
    return;
  }
  if (event.kind == AUDIO::PLUGIN::Event::NOTE_ON && event.value > 0)
    CORE::PERCUSSION::strike(voice.clap, event.value);
}
