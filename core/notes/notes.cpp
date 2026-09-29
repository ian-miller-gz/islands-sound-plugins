// SPDX-License-Identifier: AGPL-3.0-or-later
#include "notes.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float UNIT = 1.0f;
constexpr Float HALF = 0.5f;

}  // namespace

auto SOUND::PLUGINS::CORE::NOTES::struck(const AUDIO::PLUGIN::Event &event)
  -> Flag {
  return event.kind == AUDIO::PLUGIN::Event::NOTE_ON && event.value > 0;
}

auto SOUND::PLUGINS::CORE::NOTES::lifted(const AUDIO::PLUGIN::Event &event)
  -> Flag {
  if (event.kind == AUDIO::PLUGIN::Event::NOTE_OFF) return true;
  return event.kind == AUDIO::PLUGIN::Event::NOTE_ON && event.value <= 0;
}

auto SOUND::PLUGINS::CORE::NOTES::placed(Integer pitch) -> Whole {
  if (pitch < 0 || pitch > static_cast<Integer>(HIGHEST)) return SILENT;
  return static_cast<Whole>(pitch);
}

auto SOUND::PLUGINS::CORE::NOTES::put(
  Out &out, const AUDIO::PLUGIN::Event &event) -> Flag {
  if (out.written >= out.room) return false;
  out.events[out.written++] = event;
  return true;
}

auto SOUND::PLUGINS::CORE::NOTES::drain(
  Out &held, AUDIO::PLUGIN::Event *out, Whole room) -> Whole {
  const Whole said = held.written < room ? held.written : room;
  for (Whole at = 0; at < said; ++at) out[at] = held.events[at];
  held.written = 0;
  return said;
}

void SOUND::PLUGINS::CORE::NOTES::strike(
  Tally &tally, Out &out, Whole pitch, Whole offset, Float velocity) {
  if (pitch > HIGHEST) return;
  if (put(out, {AUDIO::PLUGIN::Event::NOTE_ON, offset, pitch, velocity}))
    ++tally.counts[pitch];
}

void SOUND::PLUGINS::CORE::NOTES::lift(
  Tally &tally, Out &out, Whole pitch, Whole offset) {
  if (pitch > HIGHEST || tally.counts[pitch] == 0) return;
  if (--tally.counts[pitch] > 0) return;
  put(out, {AUDIO::PLUGIN::Event::NOTE_OFF, offset, pitch, 0});
}

void SOUND::PLUGINS::CORE::NOTES::sow(NOISE::White &white, Float seed) {
  const NOISE::Register grain = static_cast<NOISE::Register>(seed) + 1u;
  NOISE::seed(white, NOISE::SEED * grain);
}

auto SOUND::PLUGINS::CORE::NOTES::draw(NOISE::White &white) -> Float {
  return (NOISE::tick(white) + ::UNIT) * ::HALF;
}
