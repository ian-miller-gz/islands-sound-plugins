// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float TEN = 10.0f;
constexpr Float DECIBELS = 20.0f;
constexpr Float FLOOR = 10.0f;

auto soft(SHAPER::Stage &stage, Float in) -> Float {
  return CORE::SHAPER::tick(stage.oversampler, in, CORE::SHAPER::soft);
}

auto hard(SHAPER::Stage &stage, Float in) -> Float {
  return CORE::SHAPER::tick(stage.oversampler, in, CORE::SHAPER::hard);
}

auto tube(SHAPER::Stage &stage, Float in) -> Float {
  return CORE::SHAPER::tick(stage.oversampler, in, CORE::SHAPER::tube);
}

auto fold(SHAPER::Stage &stage, Float in) -> Float {
  return CORE::SHAPER::tick(stage.oversampler, in, CORE::SHAPER::fold);
}

auto crush(SHAPER::Stage &stage, Float in) -> Float {
  return CORE::SHAPER::hard(CORE::SHAPER::tick(stage.crusher, in));
}

using Path = auto (*)(SHAPER::Stage &, Float) -> Float;

constexpr Path PATHS[SHAPER::CURVES] = {soft, hard, tube, fold, crush};

}  // namespace

void SOUND::SHAPER::settle(Module &module) {
  module.gain = std::pow(::TEN, module.rows[DRIVE] / ::DECIBELS);
  for (Stage &stage : module.stages) {
    CORE::SHAPER::settle(
      stage.crusher, module.rows[BITS], module.rows[RATE], module.rate);
    CORE::SHAPER::settle(stage.blocker, ::FLOOR, module.rate);
  }
}

void SOUND::SHAPER::apply(Module &module, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  module.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(module);
}

auto SOUND::SHAPER::shaped(const Module &module, Stage &stage, Float in)
  -> Float {
  const Whole curve = Whole(module.rows[CURVE]);
  const Path path = PATHS[curve < CURVES ? curve : SOFT];
  const Float wet =
    CORE::SHAPER::tick(stage.blocker, path(stage, in * module.gain));
  return in + (wet - in) * module.rows[MIX];
}
