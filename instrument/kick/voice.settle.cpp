// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float HALF = 0.5f;

}  // namespace

void SOUND::PLUGINS::KICK::settle(Voice &voice) {
  const Float *rows = voice.rows;
  const CORE::PERCUSSION::Batter batter = {
    .tune = rows[TUNE],
    .decay = rows[DECAY],
    .attack = rows[ATTACK],
    .drive = rows[DRIVE],
    .tone = rows[TONE],
    .bend = rows[BEND],
    .drop = rows[DROP],
    .slope = rows[SLOPE],
    .click = Whole(rows[CLICK] + HALF),
    .sigh = rows[SIGH],
    .low = rows[LOW],
    .high = rows[HIGH],
    .curve = rows[CURVE],
    .touch = rows[TOUCH]};
  CORE::PERCUSSION::settle(voice.kick, batter, voice.rate);
}
