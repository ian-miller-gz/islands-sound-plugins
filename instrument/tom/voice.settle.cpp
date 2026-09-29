// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr CORE::PERCUSSION::Key KEYS[] = {
  {41, CORE::PERCUSSION::HEIGHT::LOW},  {43, CORE::PERCUSSION::HEIGHT::LOW},
  {45, CORE::PERCUSSION::HEIGHT::MID},  {47, CORE::PERCUSSION::HEIGHT::MID},
  {48, CORE::PERCUSSION::HEIGHT::HIGH}, {50, CORE::PERCUSSION::HEIGHT::HIGH}};

}  // namespace

void SOUND::PLUGINS::TOM::settle(Voice &voice) {
  CORE::PERCUSSION::settle(
    voice.tom, voice.rows[TUNE], voice.rows[DECAY], voice.rows[BEND],
    voice.rows[TONE], voice.rate);
}

void SOUND::PLUGINS::TOM::apply(
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
  CORE::PERCUSSION::strike(voice.tom, height, event.value);
}
