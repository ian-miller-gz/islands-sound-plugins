// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.hpp"

#include "../modulator/modulator.hpp"

namespace {
using namespace SOUND::CORE;

constexpr Whole KINDS = AUDIO::PLUGIN::Event::PROGRAM + 1;
constexpr Float WIDTH = 2.0f;
constexpr Float HALF = 0.5f;

using Strike = void (*)(VOICE::Allocator &, Whole, Float);
using Lift = void (*)(VOICE::Allocator &, Whole);

constexpr Strike STRIKES[VOICE::MODES] = {
  VOICE::POLYPHONY::strike, VOICE::MONOPHONY::strike};
constexpr Lift LIFTS[VOICE::MODES] = {
  VOICE::POLYPHONY::lift, VOICE::MONOPHONY::lift};

auto mode(const VOICE::Allocator &allocator) -> Whole {
  return allocator.mode < VOICE::MODES ? allocator.mode : VOICE::POLY;
}

auto lifted(VOICE::Allocator &allocator, const AUDIO::PLUGIN::Event &event)
  -> Whole {
  if (event.index < PHASE::PITCHES)
    LIFTS[mode(allocator)](allocator, event.index);
  return VOICE::NONE;
}

auto struck(VOICE::Allocator &allocator, const AUDIO::PLUGIN::Event &event)
  -> Whole {
  if (event.value <= 0) return lifted(allocator, event);
  if (event.index < PHASE::PITCHES)
    STRIKES[mode(allocator)](allocator, event.index, event.value);
  return VOICE::NONE;
}

auto steered(VOICE::Allocator &, const AUDIO::PLUGIN::Event &event) -> Whole {
  return event.index;
}

auto ignored(VOICE::Allocator &, const AUDIO::PLUGIN::Event &) -> Whole {
  return VOICE::NONE;
}

using Handler = auto (*)(VOICE::Allocator &, const AUDIO::PLUGIN::Event &)
  -> Whole;

constexpr Handler HANDLERS[KINDS] = {struck,  lifted,  steered,
                                     ignored, ignored, ignored};

}  // namespace

void SOUND::CORE::VOICE::settle(Allocator &allocator, Whole rate) {
  allocator.glide.pole = MODULATOR::pole(allocator.glide.time, rate);
}

auto SOUND::CORE::VOICE::apply(
  Allocator &allocator, const AUDIO::PLUGIN::Event &event) -> Whole {
  return event.kind < ::KINDS ? ::HANDLERS[event.kind](allocator, event) : NONE;
}

auto SOUND::CORE::VOICE::tick(Note &note, const Glide &glide) -> Float {
  note.current += (Float(note.pitch) - note.current) * glide.pole;
  return note.current;
}

auto SOUND::CORE::VOICE::limit(const Allocator &allocator) -> Whole {
  const Whole count = allocator.count;
  return count == 0 ? 1 : count > VOICES ? VOICES : count;
}

auto SOUND::CORE::VOICE::stacked(const Allocator &allocator) -> Whole {
  const Whole unison = allocator.unison;
  const Whole most = limit(allocator);
  return unison == 0 ? 1 : unison > most ? most : unison;
}

void SOUND::CORE::VOICE::strike(
  Allocator &allocator, Note &note, Whole pitch, Float velocity) {
  const Flag fresh = note.struck == 0 || allocator.glide.slide == LEGATO;
  note.pitch = pitch;
  note.velocity = velocity < 0 ? 0 : velocity > FULL ? FULL : velocity;
  note.held = true;
  note.sounding = true;
  note.struck = allocator.clock;
  note.change = STRUCK;
  if (fresh) note.current = Float(pitch);
}

void SOUND::CORE::VOICE::stack(
  const Allocator &allocator, Note &note, Whole position) {
  const Whole stack = stacked(allocator);
  const Float span = Float(stack > 1 ? stack - 1 : 1);
  const Float place = stack > 1 ? Float(position) * ::WIDTH / span - FULL : 0;
  note.cents = place * allocator.detune * ::HALF;
  note.pan = place * allocator.spread;
}
