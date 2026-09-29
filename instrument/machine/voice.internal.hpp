// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "machine.hpp"
#include "../../core/block/block.hpp"
#include "../../core/percussion/percussion.hpp"

namespace SOUND::PLUGINS::MACHINE {

struct Machine {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::PERCUSSION::HYBRID::Kick kick;
  CORE::PERCUSSION::HYBRID::Snare snare;
  CORE::PERCUSSION::HYBRID::Hat hat;
  CORE::PERCUSSION::Clap clap;
  CORE::PERCUSSION::HYBRID::Tom tom;
  CORE::BLOCK::Meter meter;
};

void settle(Machine &machine, Whole drum);
void settle(Machine &machine);

void apply(Machine &machine, const AUDIO::PLUGIN::Event &event);

auto mix(Machine &machine) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::MACHINE
