// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "autopan.hpp"
#include "../../core/block/block.hpp"
#include "../../core/modulator/modulator.hpp"

namespace SOUND::AUTOPAN {

constexpr Float SMOOTHING = 0.002f;
constexpr Whole SIDES = 2;
constexpr Whole LEFT = 0;
constexpr Whole RIGHT = 1;
constexpr Float ARC = 0.78539816f;
constexpr Float ROOT = 1.41421356f;
constexpr Float HALF = 0.5f;
constexpr Float UNITY = 1.0f;

inline constexpr CORE::MODULATOR::Wave WAVES[] = {
  CORE::MODULATOR::SINE, CORE::MODULATOR::TRIANGLE, CORE::MODULATOR::SQUARE};

inline constexpr Float BEATS[] = {0.25f, 0.5f, 1, 2, 4};

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  CORE::MODULATOR::Lfo lfo;
  CORE::MODULATOR::Smoother smoother;
  Float rows[PARAMETERS] = {};
  CORE::BLOCK::Meter meter;
};

void build(Effect &effect);
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::AUTOPAN
