// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "pads.hpp"
#include "../../core/block/block.hpp"
#include "../../core/percussion/percussion.hpp"

namespace SOUND::PADS {

struct Kit {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::PERCUSSION::Pad pads[PADS];
  CORE::BLOCK::Meter meter;
};

void settle(Kit &kit, Whole pad);
void settle(Kit &kit);

void apply(Kit &kit, const AUDIO::PLUGIN::Event &event);

auto mix(Kit &kit) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PADS
