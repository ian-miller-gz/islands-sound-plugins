// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr CORE::PERCUSSION::Key KEYS[] = {
  {51, CORE::PERCUSSION::RIDE},
  {53, CORE::PERCUSSION::RIDE},
  {59, CORE::PERCUSSION::RIDE}};

}  // namespace

void SOUND::CYMBAL::settle(Voice &voice) {
  CORE::PERCUSSION::settle(
    voice.cymbal, voice.rows[TUNE], voice.rows[DECAY], voice.rows[TONE],
    voice.rows[SPLASH], voice.rate);
}

void SOUND::CYMBAL::apply(Voice &voice, const AUDIO::PLUGIN::Event &event) {
  if (event.kind == AUDIO::PLUGIN::Event::CONTROLLER) {
    if (event.index >= PARAMETERS) return;
    voice.rows[event.index] =
      CORE::TABLE::clamped(SHEET, event.index, event.value);
    settle(voice);
    return;
  }
  if (event.kind != AUDIO::PLUGIN::Event::NOTE_ON || event.value <= 0) return;
  const Whole plate =
    CORE::PERCUSSION::keyed(KEYS, event.index, CORE::PERCUSSION::CRASH);
  CORE::PERCUSSION::strike(voice.cymbal, plate, event.value);
}
