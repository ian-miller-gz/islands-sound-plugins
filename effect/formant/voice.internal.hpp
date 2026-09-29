// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "formant.hpp"
#include "../../core/block/block.hpp"
#include "../../core/pitch/pitch.hpp"

namespace SOUND::PLUGINS::FORMANT {

constexpr Float WINDOW = 0.03f;
constexpr Float CENT = 100.0f;

struct Formant {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  Vector<CORE::PITCH::Formant> shifters;
  CORE::BLOCK::Meter meter;
};

void build(Formant &formant);
void settle(Formant &formant);
void apply(Formant &formant, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::FORMANT
