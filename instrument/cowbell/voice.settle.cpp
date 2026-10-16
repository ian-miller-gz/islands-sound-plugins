// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

auto SOUND::PLUGINS::COWBELL::keyed(const Voice &voice) -> Flag {
  return voice.rows[KEY] > 0;
}

void SOUND::PLUGINS::COWBELL::settle(Voice &voice) {
  const Float tune =
    keyed(voice) ? voice.rows[TUNE] + voice.shift : voice.rows[TUNE];
  CORE::PERCUSSION::settle(
    voice.cowbell, tune, voice.rows[DECAY], voice.rows[INTERVAL],
    voice.rows[TONE], voice.rate);
}

void SOUND::PLUGINS::COWBELL::apply(
  Voice &voice, const AUDIO::PLUGIN::Event &event) {
  if (event.kind == AUDIO::PLUGIN::Event::CONTROLLER) {
    if (event.index >= PARAMETERS) return;
    voice.rows[event.index] =
      CORE::TABLE::clamped(SHEET, event.index, event.value);
    settle(voice);
    return;
  }
  if (event.kind != AUDIO::PLUGIN::Event::NOTE_ON || event.value <= 0) return;
  strike(voice, event);
}
