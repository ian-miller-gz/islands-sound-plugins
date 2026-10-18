// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::PLUGINS::CLAP::settle(Voice &voice) {
  const Float *rows = voice.rows;
  const CORE::PERCUSSION::Hand hand = {
    .spread = rows[SPREAD],
    .decay = rows[DECAY],
    .tone = rows[TONE],
    .tail = rows[TAIL],
    .cutoff = rows[CUTOFF],
    .emphasis = rows[EMPHASIS],
    .bursts = Whole(rows[BURSTS]),
    .scatter = rows[SCATTER],
    .curve = rows[CURVE],
    .touch = rows[TOUCH]};
  CORE::PERCUSSION::settle(voice.clap, hand, voice.rate);
}

void SOUND::PLUGINS::CLAP::apply(
  Voice &voice, const AUDIO::PLUGIN::Event &event) {
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
