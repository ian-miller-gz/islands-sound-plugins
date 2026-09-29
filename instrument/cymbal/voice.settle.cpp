// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr CORE::PERCUSSION::Key KEYS[] = {
  {51, CORE::PERCUSSION::PLATE::RIDE},
  {53, CORE::PERCUSSION::PLATE::RIDE},
  {59, CORE::PERCUSSION::PLATE::RIDE}};

}  // namespace

void SOUND::PLUGINS::CYMBAL::settle(Voice &voice) {
  CORE::PERCUSSION::settle(
    voice.cymbal, voice.rows[TUNE], voice.rows[DECAY], voice.rows[TONE],
    voice.rows[SPLASH], voice.rate);
}

void SOUND::PLUGINS::CYMBAL::apply(
  Voice &voice, const AUDIO::PLUGIN::Event &event) {
  if (event.kind == AUDIO::PLUGIN::Event::CONTROLLER) {
    if (event.index >= PARAMETERS) return;
    voice.rows[event.index] =
      CORE::TABLE::clamped(SHEET, event.index, event.value);
    settle(voice);
    return;
  }
  if (event.kind != AUDIO::PLUGIN::Event::NOTE_ON || event.value <= 0) return;
  const Whole plate =
    CORE::PERCUSSION::keyed(KEYS, event.index, CORE::PERCUSSION::PLATE::CRASH);
  CORE::PERCUSSION::strike(voice.cymbal, plate, event.value);
}
