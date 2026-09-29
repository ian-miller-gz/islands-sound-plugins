// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float UNIT = 1.0f;

auto beats(const STEPS::Sequencer &sequencer) -> Float {
  const Whole division = Whole(sequencer.rows[STEPS::DIVISION]);
  return STEPS::BEATS
    [division < STEPS::DIVISIONS ? division : STEPS::SIXTEENTH];
}

}  // namespace

void SOUND::PLUGINS::STEPS::settle(Sequencer &sequencer) {
  const Float lean = sequencer.rows[SWING] / EVEN - ::UNIT;
  CORE::CLOCK::settle(
    sequencer.clock, sequencer.rows[TEMPO], ::beats(sequencer), lean,
    sequencer.rate);
}

void SOUND::PLUGINS::STEPS::run(Sequencer &sequencer, Whole frame) {
  const Flag on = sequencer.rows[RUN] > HALF;
  if (on == sequencer.clock.running) return;
  if (!on) {
    CORE::CLOCK::stop(sequencer.clock);
    return silence(sequencer, frame);
  }
  CORE::CLOCK::reset(sequencer.clock);
  CORE::CLOCK::run(sequencer.clock);
}

void SOUND::PLUGINS::STEPS::apply(
  Sequencer &sequencer, const AUDIO::PLUGIN::Event &event) {
  if (event.kind == AUDIO::PLUGIN::Event::NOTE_ON && event.value > 0) {
    silence(sequencer, event.offset);
    return CORE::CLOCK::reset(sequencer.clock);
  }
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  sequencer.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(sequencer);
  if (event.index == RUN) run(sequencer, event.offset);
}
