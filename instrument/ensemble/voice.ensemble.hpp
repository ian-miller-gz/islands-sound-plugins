// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "ensemble.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/line/line.hpp"
#include "../../core/modulator/modulator.hpp"

namespace SOUND::PLUGINS::ENSEMBLE {

constexpr Whole LINES = 3;

struct Pair {
  Float left = 0;
  Float right = 0;
  Float whole = 0;
};

struct Ensemble {
  CORE::FILTER::Pole bucket;
  CORE::LINE::Line line;
  CORE::LINE::Sweep sweeps[LINES];
  CORE::MODULATOR::Lfo slow[LINES];
  CORE::MODULATOR::Lfo fast[LINES];
  Float wet = 0;
  Float chorus = 0;
  Float vibrato = 0;
};

void build(Ensemble &ensemble, Whole rate);
void settle(Ensemble &ensemble, const Float *rows, Whole rate);
auto tick(Ensemble &ensemble, Float in) -> Pair;

}  // namespace SOUND::PLUGINS::ENSEMBLE
