// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "doubler.hpp"
#include "../../core/block/block.hpp"
#include "../../core/line/line.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/pitch/pitch.hpp"

namespace SOUND::DOUBLER {

constexpr Whole COPIES = 4;
constexpr Float WINDOW = 0.04f;
constexpr Float SWING = 1.5f;
constexpr Float MILLI = 1000.0f;

struct Seat {
  Float detune;
  Float delay;
  Float pan;
  Float hertz;
};

inline constexpr Seat SEATS[COPIES] = {
  {-1.0f, 1.0f, -1.0f, 1.3f},
  {1.0f, 0.7f, 1.0f, 1.7f},
  {-0.5f, 1.3f, -0.5f, 2.3f},
  {0.5f, 0.5f, 0.5f, 2.9f}};

struct Copy {
  CORE::PITCH::Grains grains;
  CORE::MODULATOR::Lfo wobble;
  Float delay = 0;
  Float left = 0;
  Float right = 0;
};

struct Doubler {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::LINE::Line line;
  Copy copies[COPIES];
  Whole count = COPIES;
  Float scale = 1;
  Float swing = 0;
  CORE::BLOCK::Meter meter;
};

void build(Doubler &doubler);
void settle(Doubler &doubler);
void apply(Doubler &doubler, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::DOUBLER
