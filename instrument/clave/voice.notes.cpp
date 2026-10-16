// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float ROOT = 60;
constexpr CORE::PERCUSSION::Key KEYS[] = {{37, CORE::PERCUSSION::CLICK::RIM}};

}  // namespace

void SOUND::PLUGINS::CLAVE::strike(
  Voice &voice, const AUDIO::PLUGIN::Event &event) {
  const Float level = CORE::PERCUSSION::struck(event.value, voice.rows[CURVE]);
  const Float ring =
    voice.rows[RING] * CORE::ENVELOPE::weigh(level, voice.rows[TOUCH]);
  if (keyed(voice)) {
    voice.ratio = shifted(Float(event.index) - ROOT);
    settle(voice);
    CORE::PERCUSSION::strike(
      voice.clave, CORE::PERCUSSION::CLICK::CLAVE, level, ring);
    return;
  }
  const Whole click =
    CORE::PERCUSSION::keyed(KEYS, event.index, CORE::PERCUSSION::CLICK::CLAVE);
  CORE::PERCUSSION::strike(voice.clave, click, level, ring);
}
