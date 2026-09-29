// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr CORE::PERCUSSION::Key KEYS[] = {{37, CORE::PERCUSSION::CLICK::RIM}};

}  // namespace

void SOUND::PLUGINS::CLAVE::settle(Voice &voice) {
  CORE::PERCUSSION::settle(
    voice.clave, voice.rows[TUNE], voice.rows[DECAY], voice.rate);
}

void SOUND::PLUGINS::CLAVE::apply(
  Voice &voice, const AUDIO::PLUGIN::Event &event) {
  if (event.kind == AUDIO::PLUGIN::Event::CONTROLLER) {
    if (event.index >= PARAMETERS) return;
    voice.rows[event.index] =
      CORE::TABLE::clamped(SHEET, event.index, event.value);
    settle(voice);
    return;
  }
  if (event.kind != AUDIO::PLUGIN::Event::NOTE_ON || event.value <= 0) return;
  const Whole click =
    CORE::PERCUSSION::keyed(KEYS, event.index, CORE::PERCUSSION::CLICK::CLAVE);
  CORE::PERCUSSION::strike(voice.clave, click, event.value);
}
