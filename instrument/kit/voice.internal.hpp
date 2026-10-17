// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "kit.hpp"
#include "../../core/block/block.hpp"
#include "../../core/percussion/percussion.hpp"
#include "../../core/stereo/stereo.hpp"

namespace SOUND::PLUGINS::KIT {

struct Pad {
  Whole pitch;
  Whole drum;
  Whole part;
};

struct Kit {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::PERCUSSION::Kick kick;
  CORE::PERCUSSION::Snare snare;
  CORE::PERCUSSION::Hat hat;
  CORE::PERCUSSION::Clap clap;
  CORE::PERCUSSION::Tom tom;
  CORE::PERCUSSION::Cymbal cymbal;
  CORE::PERCUSSION::Cowbell cowbell;
  CORE::PERCUSSION::Clave clave;
  CORE::PERCUSSION::Conga conga;
  CORE::STEREO::Pair sides[DRUMS];
  CORE::BLOCK::Meter meter;
};

auto drummed(Whole index) -> Whole;
void settle(Kit &kit, Whole drum);
void settle(Kit &kit);
auto panned(Whole index) -> Whole;
void pan(Kit &kit, Whole drum);

auto found(Whole pitch) -> const Pad *;

void apply(Kit &kit, const AUDIO::PLUGIN::Event &event);

auto mix(Kit &kit) -> CORE::STEREO::Pair;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::KIT
