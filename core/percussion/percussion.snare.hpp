// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"

namespace SOUND::PLUGINS::CORE::PERCUSSION {

namespace SNARE {

constexpr Float DROP = 0.02f;
constexpr Whole MODES = 5;
constexpr Float RATIOS[MODES] = {1.0f, 1.59f, 2.14f, 2.30f, 2.65f};
constexpr Float WEIGHTS[MODES] = {1.0f, 0.8f, 0.65f, 0.55f, 0.45f};

struct Setting {
  Float tune = 0;
  Float tone = 0;
  Float snappy = 0;
  Float decay = 0;
  Float bend = 0;
  Float drop = DROP;
  Float hold = 0;
  Float body = 0;
};

}  // namespace SNARE

struct Snare {
  Sweep low;
  Sweep high;
  Sweep modes[SNARE::MODES];
  NOISE::White white;
  FILTER::Biquad band;
  FILTER::Pole floor;
  ENVELOPE::Envelope rattle;
  ENVELOPE::Gate gate;
  SNARE::Setting setting;
  Whole rate = 0;
  Whole hold = 0;
  Whole holding = 0;
  Float velocity = 0;
};

void settle(
  Snare &snare, Float tune, Float tone, Float snappy, Float decay, Whole rate);
void settle(Snare &snare, const SNARE::Setting &setting, Whole rate);
void strike(Snare &snare, Float velocity);
auto tick(Snare &snare) -> Float;
auto sounding(const Snare &snare) -> Flag;

}  // namespace SOUND::PLUGINS::CORE::PERCUSSION
