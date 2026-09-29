// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr CORE::PERCUSSION::Key KEYS[] = {
  {62, CORE::PERCUSSION::HEIGHT::HIGH},
  {63, CORE::PERCUSSION::HEIGHT::MID},
  {64, CORE::PERCUSSION::HEIGHT::LOW}};

}  // namespace

void SOUND::PLUGINS::CONGA::settle(Voice &voice) {
  CORE::PERCUSSION::settle(
    voice.conga, voice.rows[TUNE], voice.rows[DECAY], voice.rows[BEND],
    voice.rate);
}

void SOUND::PLUGINS::CONGA::apply(
  Voice &voice, const AUDIO::PLUGIN::Event &event) {
  if (event.kind == AUDIO::PLUGIN::Event::CONTROLLER) {
    if (event.index >= PARAMETERS) return;
    voice.rows[event.index] =
      CORE::TABLE::clamped(SHEET, event.index, event.value);
    settle(voice);
    return;
  }
  if (event.kind != AUDIO::PLUGIN::Event::NOTE_ON || event.value <= 0) return;
  const Whole height =
    CORE::PERCUSSION::keyed(KEYS, event.index, CORE::PERCUSSION::HEIGHT::MID);
  CORE::PERCUSSION::strike(voice.conga, height, event.value);
}
