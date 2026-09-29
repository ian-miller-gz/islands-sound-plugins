// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr PERCUSSION::HYBRID::Bend DROP = {
  .depth = 1.2f, .fast = 0.012f, .knee = 0.35f, .slow = 0.15f};
constexpr Float RATIOS[PERCUSSION::HEIGHT::HEIGHTS] = {1.0f, 1.4f, 1.9f};
constexpr Float SKIN = 0.03f;
constexpr Float DULL = 5000.0f;
constexpr Float STICK = 0.3f;

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::settle(
  Tom &tom, Float tune, Float decay, Whole rate) {
  settle(tom.body, DROP, rate);
  for (Whole height = 0; height < HEIGHT::HEIGHTS; ++height)
    tom.paces[height] = pace(tune * RATIOS[height], rate);
  shape(tom.decay, decay, rate);
  shape(tom.skin, SKIN, rate);
  FILTER::settle(tom.tone, FILTER::LOW, DULL, rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::strike(
  Tom &tom, Whole height, Float velocity) {
  tom.velocity = struck(velocity);
  strike(tom.body, tom.paces[height < HEIGHT::HEIGHTS ? height : HEIGHT::MID]);
  ENVELOPE::strike(tom.gate, tom.decay, tom.velocity);
  ENVELOPE::strike(tom.hit, tom.skin, tom.velocity);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::tick(Tom &tom) -> Float {
  if (!ENVELOPE::sounding(tom.gate)) return 0;
  const Float body = tick(tom.body) * ENVELOPE::tick(tom.gate, tom.decay);
  const Float skin = ENVELOPE::tick(tom.hit, tom.skin) * STICK;
  const Float noise = FILTER::tick(tom.tone, NOISE::tick(tom.white)) * skin;
  return SHAPER::soft((body + noise) * tom.velocity);
}
