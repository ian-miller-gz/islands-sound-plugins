// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.hpp"

#include "../modulator/modulator.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Whole KINDS = AUDIO::PLUGIN::Event::PROGRAM + 1;
constexpr Float WIDTH = 2.0f;
constexpr Float HALF = 0.5f;

auto lifted(VOICE::Allocator &allocator, const AUDIO::PLUGIN::Event &event)
  -> Whole {
  VOICE::lift(allocator, event.index);
  return VOICE::NONE;
}

auto struck(VOICE::Allocator &allocator, const AUDIO::PLUGIN::Event &event)
  -> Whole {
  if (event.value <= 0) return lifted(allocator, event);
  VOICE::strike(allocator, event.index, event.value);
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

void SOUND::PLUGINS::CORE::VOICE::settle(Allocator &allocator, Whole rate) {
  allocator.glide.pole = MODULATOR::pole(allocator.glide.time, rate);
}

auto SOUND::PLUGINS::CORE::VOICE::apply(
  Allocator &allocator, const AUDIO::PLUGIN::Event &event) -> Whole {
  return event.kind < ::KINDS ? ::HANDLERS[event.kind](allocator, event) : NONE;
}

auto SOUND::PLUGINS::CORE::VOICE::limit(const Allocator &allocator) -> Whole {
  const Whole count = allocator.count;
  return count == 0 ? 1 : count > VOICES ? VOICES : count;
}

auto SOUND::PLUGINS::CORE::VOICE::stacked(const Allocator &allocator) -> Whole {
  const Whole unison = allocator.unison;
  const Whole most = limit(allocator);
  return unison == 0 ? 1 : unison > most ? most : unison;
}

void SOUND::PLUGINS::CORE::VOICE::strike(
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

void SOUND::PLUGINS::CORE::VOICE::stack(
  const Allocator &allocator, Note &note, Whole position) {
  const Whole stack = stacked(allocator);
  const Float span = Float(stack > 1 ? stack - 1 : 1);
  const Float place = stack > 1 ? Float(position) * ::WIDTH / span - FULL : 0;
  note.cents = place * allocator.detune * ::HALF;
  note.pan = place * allocator.spread;
}
