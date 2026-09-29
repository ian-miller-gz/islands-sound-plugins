// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.hpp"

namespace {
using namespace SOUND::CORE;

auto older(const VOICE::Note &note, const VOICE::Note &victim) -> Flag {
  return note.struck < victim.struck;
}

auto quieter(const VOICE::Note &note, const VOICE::Note &victim) -> Flag {
  if (note.level == victim.level) return older(note, victim);
  return note.level < victim.level;
}

using Worse = auto (*)(const VOICE::Note &, const VOICE::Note &) -> Flag;

constexpr Worse WORSE[VOICE::STEALS] = {older, quieter};

auto worse(const VOICE::Allocator &allocator) -> Worse {
  return WORSE
    [allocator.steal < VOICE::STEALS ? allocator.steal : VOICE::OLDEST];
}

auto claim(VOICE::Allocator &allocator, Whole pitch) -> VOICE::Note & {
  const Worse judge = worse(allocator);
  VOICE::Note *same = nullptr;
  VOICE::Note *idle = nullptr;
  VOICE::Note *victim = nullptr;
  for (Whole at = 0; at < VOICE::limit(allocator); ++at) {
    VOICE::Note &note = allocator.notes[at];
    if (note.struck == allocator.clock) continue;
    if (same == nullptr && note.sounding && note.pitch == pitch) same = &note;
    if (idle == nullptr && !note.sounding) idle = &note;
    if (victim == nullptr || judge(note, *victim)) victim = &note;
  }
  return same != nullptr ? *same : idle != nullptr ? *idle : *victim;
}

}  // namespace

void SOUND::CORE::VOICE::POLYPHONY::strike(
  Allocator &allocator, Whole pitch, Float velocity) {
  ++allocator.clock;
  for (Whole position = 0; position < stacked(allocator); ++position) {
    Note &note = ::claim(allocator, pitch);
    VOICE::strike(allocator, note, pitch, velocity);
    stack(allocator, note, position);
  }
}

void SOUND::CORE::VOICE::POLYPHONY::lift(Allocator &allocator, Whole pitch) {
  for (Note &note : allocator.notes) {
    if (!note.held || note.pitch != pitch) continue;
    note.held = false;
    note.change = LIFTED;
  }
}
