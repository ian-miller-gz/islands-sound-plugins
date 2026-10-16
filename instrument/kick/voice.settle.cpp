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
