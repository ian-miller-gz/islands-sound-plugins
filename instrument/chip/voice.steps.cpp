// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void step(CHIP::Steps &steps, const CHIP::Shape &shape) {
  if (shape.decay == 0) {
    steps.level = steps.held ? shape.level : 0;
    return;
  }
  const Whole floor = steps.held ? shape.sustain : 0;
  if (++steps.count < shape.decay) return;
  steps.count = 0;
  if (steps.level > floor) --steps.level;
}

}  // namespace

auto SOUND::PLUGINS::CHIP::tick(Clock &clock) -> Flag {
  clock.phase += clock.step;
  return clock.phase < clock.step;
}

void SOUND::PLUGINS::CHIP::strike(Synth &synth) {
  for (Whole at = 0; at < ENVELOPES; ++at)
    synth.steps[at] = {.level = synth.shapes[at].level, .held = true};
  synth.arpeggio.phase = 0;
  synth.position = 0;
  synth.gated = true;
  synth.stale = true;
}

void SOUND::PLUGINS::CHIP::lift(Synth &synth) {
  for (Whole at = 0; at < ENVELOPES; ++at) {
    synth.steps[at].held = false;
    if (synth.shapes[at].decay == 0) synth.steps[at].level = 0;
  }
  synth.gated = false;
}

void SOUND::PLUGINS::CHIP::clock(Synth &synth) {
  if (tick(synth.frame))
    for (Whole at = 0; at < ENVELOPES; ++at)
      ::step(synth.steps[at], synth.shapes[at]);
  if (!tick(synth.arpeggio)) return;
  synth.position = (synth.position + 1) % POSITIONS;
  synth.stale = true;
}
