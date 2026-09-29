// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr CORE::PERCUSSION::Key KEYS[] = {
  {62, CORE::PERCUSSION::HIGH},
  {63, CORE::PERCUSSION::MID},
  {64, CORE::PERCUSSION::LOW}};

}  // namespace

void SOUND::CONGA::settle(Voice &voice) {
  CORE::PERCUSSION::settle(
    voice.conga, voice.rows[TUNE], voice.rows[DECAY], voice.rows[BEND],
    voice.rate);
}

void SOUND::CONGA::apply(Voice &voice, const AUDIO::PLUGIN::Event &event) {
  if (event.kind == AUDIO::PLUGIN::Event::CONTROLLER) {
    if (event.index >= PARAMETERS) return;
    voice.rows[event.index] =
      CORE::TABLE::clamped(SHEET, event.index, event.value);
    settle(voice);
    return;
  }
  if (event.kind != AUDIO::PLUGIN::Event::NOTE_ON || event.value <= 0) return;
  const Whole height =
    CORE::PERCUSSION::keyed(KEYS, event.index, CORE::PERCUSSION::MID);
  CORE::PERCUSSION::strike(voice.conga, height, event.value);
}
