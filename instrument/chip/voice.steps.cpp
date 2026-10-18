// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto struck(const CHIP::Shape &shape, Float weight) -> Whole {
  return Whole(std::round(Float(shape.level) * weight));
}

void step(CHIP::Steps &steps, const CHIP::Shape &shape, Float weight) {
  if (shape.decay == 0) {
    steps.level = steps.held ? struck(shape, weight) : 0;
    return;
  }
  const Whole floor = steps.held ? shape.sustain : 0;
  if (++steps.count < shape.decay) return;
  steps.count = 0;
  if (steps.level > floor) --steps.level;
}

void quarter(CHIP::Synth &synth) {
  for (Whole at = 0; at < CHIP::ENVELOPES; ++at)
    step(synth.steps[at], synth.shapes[at], synth.weight);
  CORE::VOICE::tick(synth.allocator.notes[0], synth.allocator.glide);
  synth.wobble = CORE::MODULATOR::tick(synth.vibrato);
  if (++synth.quarters % (CHIP::QUARTER / CHIP::SWEEPING) == 0)
    CHIP::sweep(synth);
  synth.stale = true;
}

}  // namespace

auto SOUND::PLUGINS::CHIP::tick(Clock &clock) -> Flag {
  clock.phase += clock.step;
  return clock.phase < clock.step;
}

void SOUND::PLUGINS::CHIP::strike(Synth &synth) {
  synth.weight = CORE::ENVELOPE::weigh(
    synth.allocator.notes[0].velocity, synth.rows[VELOCITY]);
  for (Whole at = 0; at < ENVELOPES; ++at)
    synth.steps[at] = {
      .level = ::struck(synth.shapes[at], synth.weight), .held = true};
  synth.arpeggio.phase = 0;
  synth.position = 0;
  CORE::MODULATOR::reset(synth.vibrato);
  synth.wobble = 0;
  arm(synth);
  synth.gated = true;
  synth.stale = true;
}

void SOUND::PLUGINS::CHIP::lift(Synth &synth) {
  for (Whole at = 0; at < ENVELOPES; ++at) {
    synth.steps[at].held = false;
    if (synth.shapes[at].decay == 0) synth.steps[at].level = 0;
  }
  synth.gated = false;
  synth.stale = true;
}

void SOUND::PLUGINS::CHIP::clock(Synth &synth) {
  if (tick(synth.frame)) ::quarter(synth);
  if (tick(synth.arpeggio)) synth.position = (synth.position + 1) % POSITIONS;
}
