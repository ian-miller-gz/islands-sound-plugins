// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float CENT = 100;
constexpr Float FULL = 1;
constexpr Float SHORTEST = 2;
constexpr Float SPAN = 6;
constexpr Float THOUSANDTH = 0.001f;
constexpr Float BLOCKED = 10;
constexpr Float FADING = 0.05f;
constexpr Float LIFT = 12;
constexpr Float DEEPEST = 0;
constexpr Whole SPARE = 1;
constexpr Float AIR = 100;
constexpr Float PLATE = 220;
constexpr Float WIDE = 1.5f;
constexpr Float NARROW = 2.5f;

struct Mode {
  Float hertz;
  Float q;
};

constexpr Mode MODES[] = {{AIR, WIDE}, {PLATE, NARROW}};
static_assert(std::size(MODES) == PLUCK::BODIES);

auto decay(Float seconds, Float hertz) -> Float {
  return std::pow(THOUSANDTH, FULL / (seconds * hertz));
}

void resonate(PLUCK::Synth &synth) {
  const Float *rows = synth.rows;
  for (auto &side : synth.bodies)
    for (Whole at = 0; at < PLUCK::BODIES; ++at)
      CORE::FILTER::settle(
        side[at], CORE::FILTER::PEAK, MODES[at].hertz * rows[PLUCK::SIZE],
        MODES[at].q, LIFT * rows[PLUCK::BODY], synth.rate);
}

}  // namespace

void SOUND::PLUGINS::PLUCK::build(Synth &synth) {
  const Float lowest = DEEPEST - CENTS / CENT - SEMITONES;
  const Float longest = Float(synth.rate) / CORE::PHASE::hertz(lowest);
  CORE::OSCILLATOR::build(synth.table);
  for (Voice &voice : synth.voices) {
    CORE::LINE::build(voice.string, Whole(longest) + SPARE);
    CORE::LINE::build(voice.pick, Whole(longest) + SPARE);
    build(voice);
  }
}

void SOUND::PLUGINS::PLUCK::tune(Synth &synth, Whole at) {
  Voice &voice = synth.voices[at];
  const Float *rows = synth.rows;
  const Float key = Float(synth.allocator.notes[at].pitch);
  const Float hertz = CORE::PHASE::hertz(key + synth.tune);
  voice.gains = CORE::STEREO::pan(CORE::STEREO::place(key, synth.spread));
  const Float octaves = FULL + SPAN * (FULL - rows[DAMPING]);
  CORE::LINE::settle(voice.loop, 0, hertz * std::exp2(octaves), synth.rate);
  const Float lag = voice.loop.pole / (FULL - voice.loop.pole);
  const Float period = Float(synth.rate) / hertz;
  const Float room = period - lag - ::SHORTEST;
  const Float stretch = stiffen(voice, rows[STIFFNESS], period, room);
  const Float left = period - lag - stretch;
  voice.delay = left < ::SHORTEST ? ::SHORTEST : left;
  voice.notch = period * rows[POSITION];
  voice.feedbacks[RINGING] = ::decay(rows[DECAY], hertz);
  voice.feedbacks[DAMPED] = ::decay(rows[RELEASE], hertz);
}

void SOUND::PLUGINS::PLUCK::settle(Synth &synth) {
  synth.tune = synth.rows[TUNE] / CENT + synth.rows[BEND];
  synth.fall = std::exp(-FULL / (FADING * Float(synth.rate)));
  ::resonate(synth);
  synth.spread = synth.channels < SIDES ? 0 : synth.rows[WIDTH];
  for (CORE::SHAPER::Blocker &blocker : synth.blockers)
    CORE::SHAPER::settle(blocker, BLOCKED, synth.rate);
  for (Whole at = 0; at < VOICES; ++at)
    if (synth.allocator.notes[at].sounding) tune(synth, at);
}
