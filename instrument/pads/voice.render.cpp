// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

auto SOUND::PLUGINS::PADS::mix(Kit &kit) -> Float {
  Float sum = 0;
  for (Whole pad = 0; pad < PADS; ++pad)
    sum += CORE::PERCUSSION::tick(kit.pads[pad]) * kit.rows[place(pad, LEVEL)];
  return CORE::SHAPER::soft(sum);
}

void SOUND::PLUGINS::PADS::render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &kit = *static_cast<Kit *>(instance);
  const auto sound = [&kit] { return mix(kit); };
  const auto take = [&kit](const AUDIO::PLUGIN::Event &event) {
    apply(kit, event);
  };
  const Float peak = CORE::PERCUSSION::play(
    lanes, kit.channels, frames, events, count, sound, take);
  CORE::BLOCK::publish(kit.meter, peak);
}
