// SPDX-License-Identifier: AGPL-3.0-or-later
#include <algorithm>
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Integer OCTAVE = 12;
constexpr Integer ADJACENT = 1;
constexpr Float SPILL = -40;
constexpr Float NEAR = 1;
constexpr Float FAR = 0.5f;

struct Reach {
  Integer offset;
  Float gain;
};

constexpr Reach REACHES[] = {
  {-OCTAVE, NEAR}, {OCTAVE, NEAR}, {-ADJACENT, FAR}, {ADJACENT, FAR}};
static_assert(std::size(REACHES) == DRAWBAR::NEIGHBOURS);

auto neighbour(Whole wheel, const Reach &reach) -> DRAWBAR::Neighbour {
  const Integer at = Integer(wheel) + reach.offset;
  if (at < 0 || at >= Integer(DRAWBAR::WHEELS)) return {};
  return {Whole(at), reach.gain};
}

}  // namespace

void SOUND::PLUGINS::DRAWBAR::build(Leaks &leaks) {
  for (Whole wheel = 0; wheel < WHEELS; ++wheel)
    for (Whole at = 0; at < NEIGHBOURS; ++at)
      leaks.neighbours[wheel][at] = ::neighbour(wheel, ::REACHES[at]);
}

void SOUND::PLUGINS::DRAWBAR::leak(Organ &organ) {
  const Float bleed = organ.rows[LEAKAGE] * CORE::DYNAMICS::gain(::SPILL);
  if (bleed <= 0) return;
  Float drawn[WHEELS];
  std::copy(std::begin(organ.targets), std::end(organ.targets), drawn);
  for (Whole wheel = 0; wheel < WHEELS; ++wheel) {
    if (drawn[wheel] <= 0) continue;
    for (const Neighbour &neighbour : organ.leaks.neighbours[wheel])
      organ.targets[neighbour.wheel] += bleed * neighbour.gain * drawn[wheel];
  }
}
