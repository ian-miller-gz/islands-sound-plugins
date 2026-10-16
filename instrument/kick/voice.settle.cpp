// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::PLUGINS::KICK::settle(Voice &voice) {
  const Float *rows = voice.rows;
  const CORE::PERCUSSION::Batter batter = {
    .tune = rows[TUNE],
    .decay = rows[DECAY],
    .attack = rows[ATTACK],
    .drive = rows[DRIVE],
    .tone = rows[TONE],
    .curve = rows[CURVE],
    .touch = rows[TOUCH]};
  CORE::PERCUSSION::settle(voice.kick, batter, voice.rate);
}

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
    CORE::PERCUSSION::strike(voice.kick, event.value);
}
