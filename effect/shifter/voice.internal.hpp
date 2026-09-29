// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "shifter.hpp"
#include "../../core/block/block.hpp"
#include "../../core/pitch/pitch.hpp"

namespace SOUND::SHIFTER {

constexpr Float UNITY = 1.0f;
constexpr Float SEMITONE = 100.0f;
constexpr Float SECOND = 1000.0f;

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  Vector<CORE::PITCH::Grains> strips;
  Float rows[PARAMETERS] = {};
  Float dry = 0;
  Float wet = UNITY;
  CORE::BLOCK::Meter meter;
};

void build(Effect &effect);
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::SHIFTER
