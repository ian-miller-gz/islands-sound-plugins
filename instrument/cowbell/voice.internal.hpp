// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "cowbell.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/percussion/percussion.hpp"

namespace SOUND::PLUGINS::COWBELL {

struct Voice {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  Float shift = 0;
  CORE::PERCUSSION::Cowbell cowbell;
  CORE::BLOCK::Meter meter;
};

auto keyed(const Voice &voice) -> Flag;

void settle(Voice &voice);

void strike(Voice &voice, const AUDIO::PLUGIN::Event &event);

void apply(Voice &voice, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::COWBELL
