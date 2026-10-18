// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"
#include "../../core/dynamics/dynamics.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float FLOOR = 10.0f;

auto gain(const SHAPER::Module &module, SHAPER::Stage &stage, Float cv)
  -> Float {
  const Float *rows = module.rows;
  const Float wanted = rows[SHAPER::DRIVE] + rows[SHAPER::AMOUNT] * cv;
  const Float decibels = wanted < 0                 ? 0
                         : wanted > SHAPER::LOUDEST ? SHAPER::LOUDEST
                                                    : wanted;
  if (decibels == stage.decibels) return stage.gain;
  stage.decibels = decibels;
  stage.gain = CORE::DYNAMICS::gain(decibels);
  return stage.gain;
}

}  // namespace

void SOUND::PLUGINS::SHAPER::settle(Module &module) {
  for (Stage &stage : module.stages) {
    CORE::SHAPER::settle(
      stage.crusher, module.rows[BITS], module.rows[RATE], module.rate);
    CORE::SHAPER::settle(stage.blocker, ::FLOOR, module.rate);
    CORE::SHAPER::settle(stage.chebyshev, module.rows[EVEN], module.rows[ODD]);
  }
}

void SOUND::PLUGINS::SHAPER::apply(
  Module &module, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  module.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(module);
}

auto SOUND::PLUGINS::SHAPER::shaped(
  const Module &module, Stage &stage, Float in, Float cv) -> Float {
  const Float driven = in * ::gain(module, stage, cv) + module.rows[BIAS];
  const Float wet =
    CORE::SHAPER::tick(stage.blocker, curved(module, stage, driven));
  return in + (wet - in) * module.rows[MIX];
}
