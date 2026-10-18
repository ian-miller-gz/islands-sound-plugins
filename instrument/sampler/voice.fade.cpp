// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"
#include "../../core/dynamics/dynamics.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float HALF = 0.5f;
constexpr Float FULL = 1;

auto root(const SAMPLER::Synth &synth, Float place) -> Float {
  const Float scaled = place * Float(SAMPLER::ROOTS);
  const Whole at = Whole(scaled < 0 ? 0 : scaled);
  if (at >= SAMPLER::ROOTS) return synth.roots[SAMPLER::ROOTS];
  const Float here = synth.roots[at];
  return here + (synth.roots[at + 1] - here) * (scaled - Float(at));
}

}  // namespace

void SOUND::PLUGINS::SAMPLER::prepare(Synth &synth) {
  for (Whole at = 1; at <= ROOTS; ++at) {
    const Float place = Float(at) / Float(ROOTS);
    synth.roots[at] =
      CORE::DYNAMICS::gain(CORE::DYNAMICS::decibels(place) * ::HALF);
  }
}

auto SOUND::PLUGINS::SAMPLER::blend(
  const Synth &synth, const Region &region, Float along) -> Blend {
  const Float from = Float(region.end) - region.fade;
  if (region.fade <= 0 || along <= from) return {};
  const Float place = (along - from) / region.fade;
  const Float into = place > ::FULL ? ::FULL : place;
  return {::root(synth, ::FULL - into), ::root(synth, into)};
}
