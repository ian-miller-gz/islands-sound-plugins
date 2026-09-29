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
  for (Whole at = 0; at < PLUCK::BODIES; ++at)
    CORE::FILTER::settle(
      synth.bodies[at], CORE::FILTER::PEAK, MODES[at].hertz * rows[PLUCK::SIZE],
      MODES[at].q, LIFT * rows[PLUCK::BODY], synth.rate);
}

}  // namespace

void SOUND::PLUGINS::PLUCK::build(Synth &synth) {
  const Float longest = Float(synth.rate) / CORE::PHASE::hertz(DEEPEST);
  for (Voice &voice : synth.voices) {
    CORE::LINE::build(voice.string, Whole(longest) + SPARE);
    CORE::LINE::build(voice.pick, Whole(longest) + SPARE);
  }
}

void SOUND::PLUGINS::PLUCK::tune(Synth &synth, Whole at) {
  Voice &voice = synth.voices[at];
  const Float *rows = synth.rows;
  const Float hertz =
    CORE::PHASE::hertz(Float(synth.allocator.notes[at].pitch) + synth.tune);
  const Float octaves = FULL + SPAN * (FULL - rows[DAMPING]);
  CORE::LINE::settle(voice.loop, 0, hertz * std::exp2(octaves), synth.rate);
  const Float lag = voice.loop.pole / (FULL - voice.loop.pole);
  const Float period = Float(synth.rate) / hertz;
  voice.delay = period - lag < SHORTEST ? SHORTEST : period - lag;
  voice.notch = period * rows[POSITION];
  voice.feedbacks[RINGING] = ::decay(rows[DECAY], hertz);
  voice.feedbacks[DAMPED] = ::decay(rows[RELEASE], hertz);
}

void SOUND::PLUGINS::PLUCK::settle(Synth &synth) {
  synth.tune = synth.rows[TUNE] / CENT;
  synth.fall = std::exp(-FULL / (FADING * Float(synth.rate)));
  ::resonate(synth);
  CORE::SHAPER::settle(synth.blocker, BLOCKED, synth.rate);
  for (Whole at = 0; at < VOICES; ++at)
    if (synth.allocator.notes[at].sounding) tune(synth, at);
}
