// SPDX-License-Identifier: AGPL-3.0-or-later
#include <algorithm>
#include <cmath>
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float OCTAVES = 4;
constexpr Float FULL = 1;

auto linear(Float octaves) -> Float { return octaves / OCTAVES; }

auto exponential(Float octaves) -> Float {
  return (std::exp2(octaves) - FULL) / (std::exp2(OCTAVES) - FULL);
}

using Reach = auto (*)(Float) -> Float;

constexpr Reach REACHES[] = {linear, exponential};
static_assert(std::size(REACHES) == std::size(OPERATOR::CURVES));

auto trait(const OPERATOR::Synth &synth, Whole unit, Whole at) -> Float {
  return synth.rows[OPERATOR::trait(unit, at)];
}

auto reached(const OPERATOR::Synth &synth, Whole unit, Float key) -> Float {
  const Float edge = ::trait(synth, unit, OPERATOR::BREAK);
  const Whole side = key < edge ? OPERATOR::LOWER : OPERATOR::UPPER;
  const Float octaves = std::fabs(key - edge) / CORE::PHASE::OCTAVE;
  const Whole curve = Whole(::trait(synth, unit, OPERATOR::CURVE));
  const Reach reach = REACHES[curve < std::size(REACHES) ? curve : 0];
  return ::trait(synth, unit, side) * std::fmin(reach(octaves), FULL);
}

auto gain(const OPERATOR::Synth &synth, Whole unit, Float key) -> Float {
  const Float output = synth.rows[OPERATOR::field(unit, OPERATOR::OUTPUT)];
  const Float level =
    std::clamp(output + ::reached(synth, unit, key), Float(0), OPERATOR::TOP);
  return CORE::ENVELOPE::loudness(
    level / OPERATOR::TOP, CORE::ENVELOPE::EXPONENTIAL);
}

void pace(
  const OPERATOR::Synth &synth, CORE::ENVELOPE::Segments &segments, Whole unit,
  Float key) {
  const Float *rows = synth.rows;
  const Float tracking = ::trait(synth, unit, OPERATOR::TRACKING);
  const Float lift = tracking * (key - OPERATOR::MIDDLE) / CORE::PHASE::OCTAVE;
  for (Whole stage = 0; stage < OPERATOR::STAGES; ++stage) {
    segments.rates[stage] =
      rows[OPERATOR::field(unit, OPERATOR::RATE + stage)] + lift;
    segments.levels[stage] =
      rows[OPERATOR::field(unit, OPERATOR::LEVEL + stage)];
  }
  segments.curve = CORE::ENVELOPE::EXPONENTIAL;
  segments.depth = rows[OPERATOR::VELOCITY] *
                   ::trait(synth, unit, OPERATOR::SENSE) / OPERATOR::GRADES;
  CORE::ENVELOPE::shape(segments, synth.rate);
}

}  // namespace

void SOUND::PLUGINS::OPERATOR::fit(Synth &synth, Whole at) {
  Voice &voice = synth.voices[at];
  const Float key = Float(synth.allocator.notes[at].pitch);
  for (Whole unit = 0; unit < OPERATORS; ++unit) {
    ::pace(synth, voice.segments[unit], unit, key);
    voice.gains[unit] = ::gain(synth, unit, key);
  }
}
