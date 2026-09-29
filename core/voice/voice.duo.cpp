// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Whole PRIORITIES[VOICE::PARTS] = {VOICE::LOW, VOICE::HIGH};

auto chosen(const VOICE::Allocator &allocator, Whole part) -> Whole {
  return VOICE::MONOPHONY::choose(allocator, PRIORITIES[part]);
}

void sing(
  VOICE::Allocator &allocator, Whole part, Float velocity, Flag legato) {
  VOICE::Note &note = allocator.notes[part];
  const Whole pitch = chosen(allocator, part);
  if (legato && note.held && note.pitch == pitch) return;
  if (legato && note.sounding) {
    note.pitch = pitch;
    note.held = true;
    note.change = VOICE::TIED;
    return;
  }
  VOICE::strike(allocator, note, pitch, velocity);
}

void follow(VOICE::Allocator &allocator, Whole part) {
  const VOICE::Note &note = allocator.notes[part];
  if (note.held && note.pitch == chosen(allocator, part)) return;
  sing(allocator, part, note.velocity, allocator.legato);
}

void release(VOICE::Allocator &allocator) {
  for (Whole part = 0; part < VOICE::PARTS; ++part) {
    VOICE::Note &note = allocator.notes[part];
    if (!note.held) continue;
    note.held = false;
    note.change = VOICE::LIFTED;
  }
}

}  // namespace

void SOUND::PLUGINS::CORE::VOICE::DUOPHONY::strike(
  Allocator &allocator, Whole pitch, Float velocity) {
  const Flag held = ::chosen(allocator, LOWER) != NONE;
  allocator.keys[pitch] = ++allocator.clock;
  for (Whole part = 0; part < PARTS; ++part)
    ::sing(allocator, part, velocity, allocator.legato && held);
}

void SOUND::PLUGINS::CORE::VOICE::DUOPHONY::lift(
  Allocator &allocator, Whole pitch) {
  allocator.keys[pitch] = 0;
  if (::chosen(allocator, LOWER) == NONE) return ::release(allocator);
  for (Whole part = 0; part < PARTS; ++part) ::follow(allocator, part);
}
