// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::CORE;

constexpr Float RATIOS[PERCUSSION::HEIGHTS] = {1.0f, 1.5f, 2.0f};
constexpr Float BEND = 1.0f;
constexpr Float SWEEP = 0.5f;
constexpr Float SKIN = 0.02f;
constexpr Float HIT = 0.4f;

}  // namespace

void SOUND::CORE::PERCUSSION::settle(
  Tom &tom, Float tune, Float decay, Float bend, Float tone, Whole rate) {
  for (Whole height = 0; height < HEIGHTS; ++height)
    tom.bases[height] = turn(tune * RATIOS[height], rate);
  settle(tom.sweep, decay, decay * SWEEP, bend * BEND, rate);
  shape(tom.skin, SKIN, rate);
  FILTER::settle(tom.tone, FILTER::LOW, tone, rate);
}

void SOUND::CORE::PERCUSSION::strike(Tom &tom, Whole height, Float velocity) {
  tom.velocity = struck(velocity);
  const Whole at = height < HEIGHTS ? height : MID;
  strike(tom.sweep, tom.bases[at], tom.velocity);
  ENVELOPE::strike(tom.hit, tom.skin, tom.velocity);
}

auto SOUND::CORE::PERCUSSION::tick(Tom &tom) -> Float {
  const Float body = tick(tom.sweep);
  const Float level = ENVELOPE::tick(tom.hit, tom.skin);
  const Float noise = FILTER::tick(tom.tone, NOISE::tick(tom.white));
  return SHAPER::soft(body + noise * level * HIT * tom.velocity);
}
