// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Tick = auto (*)(KIT::Kit &kit) -> Float;

constexpr Tick TICKS[KIT::DRUMS] = {
  [](KIT::Kit &kit) -> Float { return CORE::PERCUSSION::tick(kit.kick); },
  [](KIT::Kit &kit) -> Float { return CORE::PERCUSSION::tick(kit.snare); },
  [](KIT::Kit &kit) -> Float { return CORE::PERCUSSION::tick(kit.hat); },
  [](KIT::Kit &kit) -> Float { return CORE::PERCUSSION::tick(kit.clap); },
  [](KIT::Kit &kit) -> Float { return CORE::PERCUSSION::tick(kit.tom); },
  [](KIT::Kit &kit) -> Float { return CORE::PERCUSSION::tick(kit.cymbal); },
  [](KIT::Kit &kit) -> Float { return CORE::PERCUSSION::tick(kit.cowbell); },
  [](KIT::Kit &kit) -> Float { return CORE::PERCUSSION::tick(kit.clave); },
  [](KIT::Kit &kit) -> Float { return CORE::PERCUSSION::tick(kit.conga); }};

using Sounding = auto (*)(const KIT::Kit &kit) -> Flag;

constexpr Sounding SOUNDINGS[KIT::DRUMS] = {
  [](const KIT::Kit &kit) { return CORE::PERCUSSION::sounding(kit.kick); },
  [](const KIT::Kit &kit) { return CORE::PERCUSSION::sounding(kit.snare); },
  [](const KIT::Kit &kit) { return CORE::PERCUSSION::sounding(kit.hat); },
  [](const KIT::Kit &kit) { return CORE::PERCUSSION::sounding(kit.clap); },
  [](const KIT::Kit &kit) { return CORE::PERCUSSION::sounding(kit.tom); },
  [](const KIT::Kit &kit) { return CORE::PERCUSSION::sounding(kit.cymbal); },
  [](const KIT::Kit &kit) { return CORE::PERCUSSION::sounding(kit.cowbell); },
  [](const KIT::Kit &kit) { return CORE::PERCUSSION::sounding(kit.clave); },
  [](const KIT::Kit &kit) { return CORE::PERCUSSION::sounding(kit.conga); }};

}  // namespace

auto SOUND::PLUGINS::KIT::mix(Kit &kit) -> CORE::STEREO::Pair {
  CORE::STEREO::Pair sum;
  for (Whole drum = 0; drum < DRUMS; ++drum)
    if (::SOUNDINGS[drum](kit)) {
      const Float hit = ::TICKS[drum](kit) * kit.rows[place(drum, LEVEL)];
      CORE::STEREO::add(sum, hit, kit.sides[drum]);
    }
  return {CORE::SHAPER::soft(sum.left), CORE::SHAPER::soft(sum.right)};
}

void SOUND::PLUGINS::KIT::render(
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
