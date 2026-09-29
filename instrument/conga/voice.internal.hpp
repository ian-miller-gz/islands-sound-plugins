// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "conga.hpp"
#include "../../core/block/block.hpp"
#include "../../core/percussion/percussion.hpp"

namespace SOUND::PLUGINS::CONGA {

struct Voice {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::PERCUSSION::Conga conga;
  CORE::BLOCK::Meter meter;
};

void settle(Voice &voice);

void apply(Voice &voice, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::CONGA
