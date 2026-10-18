// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussive.hpp"
#include "../../core/block/block.hpp"
#include "../../core/percussion/percussion.hpp"

namespace SOUND::PLUGINS::PERCUSSIVE {

struct Machine {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::PERCUSSION::Pad pads[SLOTS];
  CORE::PERCUSSION::Choke choke;
  CORE::BLOCK::Meter meter;
};

auto slotted(Whole index) -> Whole;
void settle(Machine &machine, Whole slot);
void settle(Machine &machine);

void choke(Machine &machine, Whole slot);
void apply(Machine &machine, const AUDIO::PLUGIN::Event &event);

auto sound(Machine &machine) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::PERCUSSIVE
