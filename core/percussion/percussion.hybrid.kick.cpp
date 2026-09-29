// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr PERCUSSION::HYBRID::Bend DROP = {
  .depth = 5.0f, .fast = 0.004f, .knee = 0.8f, .slow = 0.07f};
constexpr Float SNAP = 0.006f;
constexpr Float CRISP = 1000.0f;
constexpr Float WIDTH = 0.0008f;
constexpr Float SPIKE = 0.5f;
constexpr Float DRIVE = 1.5f;

auto spike(PERCUSSION::HYBRID::Kick &kick) -> Float {
  if (kick.left == 0) return 0;
  --kick.left;
  return SPIKE;
}

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::settle(
  Kick &kick, Float tune, Float decay, Float attack, Whole rate) {
  settle(kick.body, DROP, rate);
  kick.pace = pace(tune, rate);
  shape(kick.decay, decay, rate);
  shape(kick.snap, SNAP, rate);
  FILTER::settle(kick.crisp, FILTER::HIGH, CRISP, rate);
  kick.width = Whole(WIDTH * Float(rate));
  kick.attack = attack;
}

void SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::strike(
  Kick &kick, Float velocity) {
  kick.velocity = struck(velocity);
  strike(kick.body, kick.pace);
  ENVELOPE::strike(kick.gate, kick.decay, kick.velocity);
  ENVELOPE::strike(kick.click, kick.snap, kick.velocity);
  kick.left = kick.width;
}

auto SOUND::PLUGINS::CORE::PERCUSSION::HYBRID::tick(Kick &kick) -> Float {
  if (!ENVELOPE::sounding(kick.gate)) return 0;
  const Float body = tick(kick.body) * ENVELOPE::tick(kick.gate, kick.decay);
  const Float snap = ENVELOPE::tick(kick.click, kick.snap);
  const Float noise = FILTER::tick(kick.crisp, NOISE::tick(kick.white)) * snap;
  const Float click = (noise + ::spike(kick)) * kick.attack;
  return SHAPER::soft((body + click) * kick.velocity * DRIVE);
}
