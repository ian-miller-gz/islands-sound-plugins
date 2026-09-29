// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::PLUGINS::TOM::render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &voice = *static_cast<Voice *>(instance);
  const auto sound = [&voice] {
    return CORE::PERCUSSION::tick(voice.tom) * voice.rows[LEVEL];
  };
  const auto take = [&voice](const AUDIO::PLUGIN::Event &event) {
    apply(voice, event);
  };
  const Float peak = CORE::PERCUSSION::play(
    lanes, voice.channels, frames, events, count, sound, take);
  CORE::BLOCK::publish(voice.meter, peak);
}
