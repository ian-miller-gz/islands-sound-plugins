// SPDX-License-Identifier: AGPL-3.0-or-later
#include "arpeggiator.internal.hpp"

namespace {
using namespace SOUND;

auto gather(ARPEGGIATOR::Arpeggio &arpeggio) -> Whole {
  Whole count = 0;
  for (Whole pitch = 0; pitch < CORE::NOTES::KEYS; ++pitch)
    if (arpeggio.velocities[pitch] > 0) arpeggio.keys[count++] = pitch;
  return count;
}

auto climbed(Whole key, Whole octave) -> Whole {
  return CORE::NOTES::placed(
    static_cast<Integer>(key + octave * CORE::NOTES::OCTAVE));
}

void strike(
  ARPEGGIATOR::Arpeggio &arpeggio, Whole frame, Whole step, Float span) {
  ARPEGGIATOR::silence(arpeggio, frame);
  const Whole count = ::gather(arpeggio);
  if (count == 0) return;
  const Whole octaves = Whole(arpeggio.rows[ARPEGGIATOR::OCTAVES]);
  const Whole at = ARPEGGIATOR::pick(arpeggio, step, count * octaves);
  const Whole key = arpeggio.keys[at % count];
  const Whole pitch = ::climbed(key, at / count);
  if (pitch == CORE::NOTES::SILENT) return;
  const Whole held = Whole(arpeggio.rows[ARPEGGIATOR::GATE] * span);
  arpeggio.left = held == 0 ? 1 : held;
  arpeggio.sounding = pitch;
  CORE::NOTES::put(
    arpeggio.out,
    {AUDIO::PLUGIN::Event::NOTE_ON, frame, pitch, arpeggio.velocities[key]});
}

void lapse(ARPEGGIATOR::Arpeggio &arpeggio, Whole frame) {
  if (arpeggio.sounding == CORE::NOTES::SILENT) return;
  if (arpeggio.left > 0) --arpeggio.left;
  if (arpeggio.left == 0) ARPEGGIATOR::silence(arpeggio, frame);
}

}  // namespace

void SOUND::ARPEGGIATOR::silence(Arpeggio &arpeggio, Whole frame) {
  if (arpeggio.sounding == CORE::NOTES::SILENT) return;
  CORE::NOTES::put(
    arpeggio.out,
    {AUDIO::PLUGIN::Event::NOTE_OFF, frame, arpeggio.sounding, 0});
  arpeggio.sounding = CORE::NOTES::SILENT;
}

void SOUND::ARPEGGIATOR::tick(Arpeggio &arpeggio, Whole frame) {
  ::lapse(arpeggio, frame);
  CORE::CLOCK::Edge edge;
  if (CORE::CLOCK::advance(arpeggio.clock, 1, &edge, 1) == 0) return;
  const Float span = CORE::CLOCK::length(arpeggio.clock, edge.step);
  ::strike(arpeggio, frame, edge.step, span);
}
