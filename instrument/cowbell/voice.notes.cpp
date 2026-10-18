// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float ROOT = 60;

}  // namespace

void SOUND::PLUGINS::COWBELL::strike(
  Voice &voice, const AUDIO::PLUGIN::Event &event) {
  const Float level = CORE::PERCUSSION::struck(event.value, voice.rows[CURVE]);
  const Float clank =
    voice.rows[CLANK] * CORE::ENVELOPE::weigh(level, voice.rows[TOUCH]);
  const Float ring = CORE::ENVELOPE::FULL - voice.rows[CLANK];
  if (keyed(voice)) {
    voice.shift = Float(event.index) - ROOT;
    settle(voice);
  }
  CORE::PERCUSSION::strike(voice.cowbell, level, clank, ring);
}
