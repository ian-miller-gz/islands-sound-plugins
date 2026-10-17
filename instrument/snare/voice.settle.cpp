// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::PLUGINS::SNARE::settle(Voice &voice) {
  const Float *rows = voice.rows;
  CORE::PERCUSSION::settle(
    voice.snare,
    CORE::PERCUSSION::SNARE::Setting{
      .tune = rows[TUNE],
      .tone = rows[TONE],
      .snappy = rows[SNAPPY],
      .decay = rows[DECAY],
      .bend = rows[BEND],
      .drop = rows[DROP],
      .hold = rows[HOLD],
      .body = rows[BODY],
      .touch = rows[TOUCH]},
    voice.rate);
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
  if (event.kind != AUDIO::PLUGIN::Event::NOTE_ON || event.value <= 0) return;
  strike(voice, event);
}
