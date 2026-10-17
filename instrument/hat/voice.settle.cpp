// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::PLUGINS::HAT::settle(Voice &voice) {
  CORE::PERCUSSION::settle(
    voice.hat, voice.rows[TUNE], voice.rows[CLOSED], voice.rows[OPEN],
    voice.rows[TONE], voice.rate);
  const CORE::PERCUSSION::Setting setting = {
    .pedal = voice.rows[PEDAL],
    .noise = voice.rows[NOISE],
    .metal = Whole(voice.rows[METAL]),
    .touch = voice.rows[TOUCH]};
  CORE::PERCUSSION::settle(voice.hat, setting, voice.rate);
}

void SOUND::PLUGINS::HAT::apply(
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
