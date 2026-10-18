// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::PLUGINS::CONGA::settle(Voice &voice) {
  const Float *rows = voice.rows;
  const CORE::PERCUSSION::Head head = {
    .tune = rows[TUNE],
    .decay = rows[DECAY],
    .bend = rows[BEND],
    .drop = rows[DROP],
    .slope = rows[SLOPE],
    .tone = rows[TONE],
    .ring = rows[RING],
    .curve = rows[CURVE]};
  CORE::PERCUSSION::settle(voice.conga, head, voice.rate);
}
