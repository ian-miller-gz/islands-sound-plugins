// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

auto last(const VOICE::Allocator &allocator) -> Whole {
  Whole chosen = VOICE::NONE;
  Whole latest = 0;
  for (Whole pitch = 0; pitch < PHASE::PITCHES; ++pitch) {
    if (allocator.keys[pitch] <= latest) continue;
    latest = allocator.keys[pitch];
    chosen = pitch;
  }
  return chosen;
}

auto low(const VOICE::Allocator &allocator) -> Whole {
  for (Whole pitch = 0; pitch < PHASE::PITCHES; ++pitch)
    if (allocator.keys[pitch] != 0) return pitch;
  return VOICE::NONE;
}

auto high(const VOICE::Allocator &allocator) -> Whole {
  for (Whole pitch = PHASE::PITCHES; pitch > 0; --pitch)
    if (allocator.keys[pitch - 1] != 0) return pitch - 1;
  return VOICE::NONE;
}

using Chooser = auto (*)(const VOICE::Allocator &) -> Whole;

constexpr Chooser CHOOSERS[VOICE::PRIORITIES] = {last, low, high};

void sing(
  VOICE::Allocator &allocator, Whole pitch, Float velocity, Flag legato) {
  for (Whole position = 0; position < VOICE::stacked(allocator); ++position) {
    VOICE::Note &note = allocator.notes[position];
    if (legato && note.sounding) {
      note.pitch = pitch;
      note.held = true;
      note.change = VOICE::TIED;
      continue;
    }
    VOICE::strike(allocator, note, pitch, velocity);
    VOICE::stack(allocator, note, position);
  }
}

}  // namespace

auto SOUND::PLUGINS::CORE::VOICE::MONOPHONY::choose(const Allocator &allocator)
  -> Whole {
  const Whole priority = allocator.priority;
  return ::CHOOSERS[priority < PRIORITIES ? priority : LAST](allocator);
}

void SOUND::PLUGINS::CORE::VOICE::MONOPHONY::strike(
  Allocator &allocator, Whole pitch, Float velocity) {
  const Flag legato = allocator.legato && choose(allocator) != NONE;
  allocator.keys[pitch] = ++allocator.clock;
  const Whole chosen = choose(allocator);
  const Note &lead = allocator.notes[0];
  if (legato && lead.held && chosen == lead.pitch) return;
  ::sing(allocator, chosen, velocity, legato);
}

void SOUND::PLUGINS::CORE::VOICE::MONOPHONY::lift(
  Allocator &allocator, Whole pitch) {
  allocator.keys[pitch] = 0;
  const Whole chosen = choose(allocator);
  const Note &lead = allocator.notes[0];
  if (chosen == NONE) {
    for (Whole position = 0; position < stacked(allocator); ++position) {
      Note &note = allocator.notes[position];
      if (!note.held) continue;
      note.held = false;
      note.change = LIFTED;
    }
    return;
  }
  if (chosen == lead.pitch && lead.held) return;
  ::sing(allocator, chosen, lead.velocity, allocator.legato);
}

auto SOUND::PLUGINS::CORE::VOICE::tick(Note &note, const Glide &glide)
  -> Float {
  note.current += (Float(note.pitch) - note.current) * glide.pole;
  return note.current;
}
