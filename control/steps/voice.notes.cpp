// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void emit(
  STEPS::Sequencer &sequencer, Whole kind, Whole frame, Whole pitch,
  Float velocity) {
  if (sequencer.written >= SOUND::PLUGIN::ROOM) return;
  sequencer.notes[sequencer.written++] = {kind, frame, pitch, velocity};
}

void strike(STEPS::Sequencer &sequencer, Whole frame, Whole step, Float span) {
  STEPS::silence(sequencer, frame);
  const Float gate = sequencer.rows[STEPS::GATE + step];
  const Float velocity = sequencer.rows[STEPS::VELOCITY + step];
  if (gate <= 0 || velocity <= 0) return;
  const Whole pitch = Whole(sequencer.rows[STEPS::PITCH + step]);
  const Whole held = Whole(gate * span);
  sequencer.left = held == 0 ? 1 : held;
  sequencer.sounding = pitch;
  ::emit(sequencer, AUDIO::PLUGIN::Event::NOTE_ON, frame, pitch, velocity);
}

void lapse(STEPS::Sequencer &sequencer, Whole frame) {
  if (sequencer.sounding == STEPS::SILENT) return;
  if (sequencer.left > 0) --sequencer.left;
  if (sequencer.left == 0) STEPS::silence(sequencer, frame);
}

}  // namespace

void SOUND::PLUGINS::STEPS::silence(Sequencer &sequencer, Whole frame) {
  if (sequencer.sounding == SILENT) return;
  ::emit(
    sequencer, AUDIO::PLUGIN::Event::NOTE_OFF, frame, sequencer.sounding, 0);
  sequencer.sounding = SILENT;
}

void SOUND::PLUGINS::STEPS::tick(Sequencer &sequencer, Whole frame) {
  ::lapse(sequencer, frame);
  CORE::CLOCK::Edge edge;
  if (CORE::CLOCK::advance(sequencer.clock, 1, &edge, 1) == 0) return;
  const Float span = CORE::CLOCK::length(sequencer.clock, edge.step);
  ::strike(sequencer, frame, edge.step % LENGTH, span);
}

auto SOUND::PLUGINS::STEPS::answer(
  void *instance, const AUDIO::PLUGIN::Event *, Whole,
  AUDIO::PLUGIN::Event *out, Whole room) -> Whole {
  auto &sequencer = *static_cast<Sequencer *>(instance);
  const Whole said = sequencer.written < room ? sequencer.written : room;
  for (Whole at = 0; at < said; ++at) out[at] = sequencer.notes[at];
  sequencer.written = 0;
  return said;
}
