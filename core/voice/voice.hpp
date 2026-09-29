// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../phase/phase.hpp"

namespace SOUND::CORE::VOICE {

constexpr Whole VOICES = 16;
constexpr Whole PHASES = 4;
constexpr Whole NONE = ~Whole(0);
constexpr Float FULL = 1.0f;

enum Mode : Whole { POLY, MONO, MODES };
enum Steal : Whole { OLDEST, QUIETEST, STEALS };
enum Priority : Whole { LAST, LOW, HIGH, PRIORITIES };
enum Slide : Whole { ALWAYS, LEGATO, SLIDES };
enum Change : Whole { KEPT, STRUCK, TIED, LIFTED };

struct Note {
  Whole pitch = 0;
  Float velocity = 0;
  Flag held = false;
  Flag sounding = false;
  Whole struck = 0;
  Float current = 0;
  Float cents = 0;
  Float pan = 0;
  Float level = 0;
  Whole change = KEPT;
  PHASE::Wheel phases[PHASES] = {};
};

struct Glide {
  Float time = 0;
  Whole slide = ALWAYS;
  Float pole = FULL;
};

struct Allocator {
  Note notes[VOICES];
  Whole count = VOICES;
  Whole mode = POLY;
  Whole steal = OLDEST;
  Whole priority = LAST;
  Flag legato = true;
  Whole unison = 1;
  Float detune = 0;
  Float spread = 0;
  Glide glide;
  Whole keys[PHASE::PITCHES] = {};
  Whole clock = 0;
};

void settle(Allocator &allocator, Whole rate);
auto apply(Allocator &allocator, const AUDIO::PLUGIN::Event &event) -> Whole;
auto tick(Note &note, const Glide &glide) -> Float;

auto limit(const Allocator &allocator) -> Whole;
auto stacked(const Allocator &allocator) -> Whole;
void strike(Allocator &allocator, Note &note, Whole pitch, Float velocity);
void stack(const Allocator &allocator, Note &note, Whole position);

template <class Visit>
void drain(Allocator &allocator, Visit &&visit) {
  for (Note &note : allocator.notes) {
    if (note.change == KEPT) continue;
    visit(note);
    note.change = KEPT;
  }
}

template <class Visit>
auto apply(
  Allocator &allocator, const AUDIO::PLUGIN::Event &event,
  Visit &&visit) -> Whole {
  const Whole row = apply(allocator, event);
  drain(allocator, visit);
  return row;
}

}  // namespace SOUND::CORE::VOICE

namespace SOUND::CORE::VOICE::POLYPHONY {
void strike(Allocator &allocator, Whole pitch, Float velocity);
void lift(Allocator &allocator, Whole pitch);
}  // namespace SOUND::CORE::VOICE::POLYPHONY

namespace SOUND::CORE::VOICE::MONOPHONY {
auto choose(const Allocator &allocator) -> Whole;
void strike(Allocator &allocator, Whole pitch, Float velocity);
void lift(Allocator &allocator, Whole pitch);
}  // namespace SOUND::CORE::VOICE::MONOPHONY
