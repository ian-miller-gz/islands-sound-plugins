// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "transient.hpp"
#include "../../core/block/block.hpp"
#include "../../core/dynamics/dynamics.hpp"

namespace SOUND::PLUGINS::TRANSIENT {

constexpr Float UNITY = 1.0f;
constexpr Float QUIET = 0.000001f;

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  CORE::DYNAMICS::Detector quick;
  CORE::DYNAMICS::Detector slow;
  Float rows[PARAMETERS] = {};
  CORE::BLOCK::Meter meter;
};

void build(Effect &effect);
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::TRANSIENT

namespace SOUND::PLUGINS::TRANSIENT::QUICK {
constexpr Float ATTACK = 0.0005f;
constexpr Float RELEASE = 0.02f;
}  // namespace SOUND::PLUGINS::TRANSIENT::QUICK

namespace SOUND::PLUGINS::TRANSIENT::SLOW {
constexpr Float ATTACK = 0.02f;
constexpr Float RELEASE = 0.2f;
}  // namespace SOUND::PLUGINS::TRANSIENT::SLOW
