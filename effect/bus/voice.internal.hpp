// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "bus.hpp"
#include "../../core/block/block.hpp"
#include "../../core/dynamics/dynamics.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/shaper/shaper.hpp"

namespace SOUND::PLUGINS::BUS {

struct Detectors {
  CORE::DYNAMICS::Detector fast;
  CORE::DYNAMICS::Detector slow;
  CORE::DYNAMICS::Detector tail;
  CORE::DYNAMICS::Detector glue;
};

struct Bus {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  Detectors detectors;
  CORE::DYNAMICS::Computer computer;
  Vector<CORE::FILTER::Biquad> tilts;
  Vector<CORE::SHAPER::Oversampler> tapes;
  Float push = 1;
  Float trim = 1;
  CORE::BLOCK::Meter meter;
};

void build(Bus &bus);
void settle(Bus &bus);
void apply(Bus &bus, const AUDIO::PLUGIN::Event &event);

auto shape(Bus &bus, Float heard) -> Float;
auto squeeze(Bus &bus, Float heard) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::BUS
