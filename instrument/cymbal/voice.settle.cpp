// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::PLUGINS::CYMBAL::settle(Voice &voice) {
  const Float *rows = voice.rows;
  const CORE::PERCUSSION::Bronze bronze = {
    .tune = rows[TUNE],
    .decay = rows[DECAY],
    .tone = rows[TONE],
    .splash = rows[SPLASH],
    .edge = rows[EDGE],
    .noise = rows[NOISE],
    .curve = rows[CURVE],
    .touch = rows[TOUCH]};
  CORE::PERCUSSION::settle(voice.cymbal, bronze, voice.rate);
}
