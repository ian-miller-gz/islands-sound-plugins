// SPDX-License-Identifier: AGPL-3.0-or-later
#include <algorithm>
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float OPEN = 0.40f;
constexpr Float SHUT = 0.45f;
constexpr Float ORDER = 8;
constexpr Float PEAK = 4;
constexpr Float WIDTH = 1.0f / 3;
constexpr Float OCTAVE = 12;
constexpr Float TINY = 0.000001f;
constexpr Float FULL = 1;

auto fade(Float hertz, Whole rate) -> Float {
  const Float open = OPEN * Float(rate);
  const Float shut = SHUT * Float(rate);
  return std::clamp((shut - hertz) / (shut - open), 0.0f, FULL);
}

auto bump(const Float *rows, Float order, Float centre) -> Float {
  const Float focus = rows[ADDITIVE::FOCUS];
  const Float distance = (order - centre) / rows[ADDITIVE::BREADTH];
  return FULL - focus + focus * std::exp(-distance * distance);
}

auto pass(Float ratio, Float resonance) -> Float {
  const Float slope = FULL / std::sqrt(FULL + std::pow(ratio, ORDER));
  const Float octaves = std::log2(ratio) / WIDTH;
  return slope * (FULL + PEAK * resonance * std::exp(-octaves * octaves));
}

auto cutoff(const Float *rows, const ADDITIVE::Voice &voice, Float velocity)
  -> Float {
  Float octaves = rows[ADDITIVE::KEYBOARD] * (voice.key - ADDITIVE::REFERENCE);
  octaves /= OCTAVE;
  octaves += rows[ADDITIVE::ENVELOPE] * voice.sweep;
  octaves += rows[ADDITIVE::TOUCH] * (velocity - FULL);
  return rows[ADDITIVE::CUTOFF] * std::exp2(octaves);
}

auto count(const Float *rows) -> Whole {
  const Whole partials = Whole(rows[ADDITIVE::PARTIALS]);
  return partials < ADDITIVE::BANK ? partials : ADDITIVE::BANK;
}

void ramp(ADDITIVE::Bank &bank, const Float *targets, Float total) {
  const Float sum = std::max(total, TINY);
  for (Whole at = 0; at < ADDITIVE::BANK; ++at)
    bank.slopes[at] = (targets[at] / sum - bank.gains[at]) / ADDITIVE::STRIDE;
}

}  // namespace

void SOUND::PLUGINS::ADDITIVE::shape(
  Synth &synth, Voice &voice, const CORE::VOICE::Note &note) {
  const Float *rows = synth.rows;
  const Float pitch = voice.key + synth.tune + rows[BEND];
  const Float root = CORE::PHASE::hertz(pitch + rows[VIBRATO] * synth.swing);
  const Float cut = ::cutoff(rows, voice, note.velocity);
  const Float centre = rows[CENTRE] + rows[SCAN] * synth.swing;
  Float targets[BANK] = {};
  Float total = 0;
  for (Whole at = 0; at < ::count(rows); ++at) {
    const Float hertz = root * synth.ratios[at];
    const Float weight =
      synth.weights[at] * ::bump(rows, Float(at + 1), centre);
    const Float faded = ::fade(hertz, synth.rate);
    total += weight;
    voice.bank.steps[at] = faded > 0 ? CORE::PHASE::step(hertz, synth.rate) : 0;
    if (faded > 0)
      targets[at] = weight * faded * ::pass(hertz / cut, rows[RESONANCE]);
  }
  ::ramp(voice.bank, targets, total);
}
