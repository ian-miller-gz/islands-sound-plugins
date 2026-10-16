// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <cstdint>

#include <threads.hpp>

#include "additive.hpp"

namespace SOUND::PLUGINS::ADDITIVE {

constexpr Whole VOICES = 8;
constexpr Whole TABLE = 4096;
constexpr Whole TURN = 20;

enum Stage : Whole { IDLE, RISING, FALLING, HELD, LEAVING };

struct Note {
  uint32_t phase = 0;
  uint32_t step = 0;
  Whole pitch = 0;
  Float velocity = 0;
  Float level = 0;
  Whole stage = IDLE;
  Whole struck = 0;
};

struct Synth {
  Whole rate = 0;
  Whole channels = 0;
  Vector<Float> table;
  Vector<uint32_t> steps;
  Float rows[PARAMETERS] = {};
  Float span = 1;
  Float rise = 0;
  Float fall = 0;
  Float drop = 0;
  Note notes[VOICES];
  Whole clock = 0;
  Float level[2] = {0, 0};
  THREADS::Shared<Whole> face{0};
};

void settle(Synth &synth);

void apply(Synth &synth, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *outputs, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::ADDITIVE
