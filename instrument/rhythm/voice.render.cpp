// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::PLUGINS::RHYTHM::render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &box = *static_cast<Box *>(instance);
  const auto sound = [&box] {
    pulse(box);
    return mix(box);
  };
  const auto take = [&box](const AUDIO::PLUGIN::Event &event) {
    apply(box, event);
  };
  const Float peak = CORE::PERCUSSION::play(
    lanes, box.channels, frames, events, count, sound, take);
  CORE::BLOCK::publish(box.meter, peak);
}
