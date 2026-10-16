// SPDX-License-Identifier: AGPL-3.0-or-later
#include <algorithm>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float OPEN = 0.40f;
constexpr Float SHUT = 0.45f;
constexpr Float TINY = 0.000001f;
constexpr Float FULL = 1;

auto fade(Float hertz, Whole rate) -> Float {
  const Float open = OPEN * Float(rate);
  const Float shut = SHUT * Float(rate);
  return std::clamp((shut - hertz) / (shut - open), 0.0f, FULL);
}

auto count(const Float *rows) -> Whole {
  const Whole partials = Whole(rows[ADDITIVE::PARTIALS]);
  return partials < ADDITIVE::BANK ? partials : ADDITIVE::BANK;
}

}  // namespace

void SOUND::PLUGINS::ADDITIVE::shape(
  Synth &synth, Voice &voice, const CORE::VOICE::Note &) {
  const Float root = CORE::PHASE::hertz(voice.key);
  Float targets[BANK] = {};
  Float total = 0;
  for (Whole at = 0; at < ::count(synth.rows); ++at) {
    const Float hertz = root * synth.ratios[at];
    const Float weight = synth.weights[at];
    const Float faded = ::fade(hertz, synth.rate);
    total += weight;
    targets[at] = weight * faded;
    voice.bank.steps[at] = faded > 0 ? CORE::PHASE::step(hertz, synth.rate) : 0;
  }
  const Float sum = std::max(total, ::TINY);
  for (Whole at = 0; at < BANK; ++at)
    voice.bank.slopes[at] =
      (targets[at] / sum - voice.bank.gains[at]) / Float(STRIDE);
}
