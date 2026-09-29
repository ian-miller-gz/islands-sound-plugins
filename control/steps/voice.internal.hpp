// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "steps.hpp"
#include "../../core/block/block.hpp"
#include "../../core/clock/clock.hpp"

namespace SOUND::STEPS {

constexpr Whole SILENT = Whole(HIGHEST) + 1;
constexpr Float HALF = 0.5f;

struct Sequencer {
  Whole rate = 0;
  Float rows[PARAMETERS] = {};
  CORE::CLOCK::Clock clock;
  Whole sounding = SILENT;
  Whole left = 0;
  AUDIO::PLUGIN::Event notes[PLUGIN::ROOM] = {};
  Whole written = 0;
};

void settle(Sequencer &sequencer);
void run(Sequencer &sequencer, Whole frame);
void apply(Sequencer &sequencer, const AUDIO::PLUGIN::Event &event);

void silence(Sequencer &sequencer, Whole frame);
void tick(Sequencer &sequencer, Whole frame);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);
auto answer(
  void *instance, const AUDIO::PLUGIN::Event *events, Whole count,
  AUDIO::PLUGIN::Event *out, Whole room) -> Whole;

}  // namespace SOUND::STEPS
