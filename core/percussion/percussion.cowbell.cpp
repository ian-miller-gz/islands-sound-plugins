// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float SEMITONE = 100.0f;
constexpr Whole PAIR = 4;
constexpr Whole BELLS = 2;
constexpr Float CENTRE = 2640.0f;
constexpr Float WIDTH = 0.5f;
constexpr Float FLAT = 0;
constexpr Float CLANK = 0.015f;
constexpr Float SHARP = 0.6f;
constexpr Float RING = 0.4f;
constexpr Float GAIN = 3.0f;

void envelop(PERCUSSION::Cowbell &cowbell, Float decay, Whole rate) {
  PERCUSSION::shape(cowbell.clank, CLANK, rate);
  PERCUSSION::shape(cowbell.tail, decay, rate);
}

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Cowbell &cowbell, Float tune, Float decay, Whole rate) {
  const Float ratio = PHASE::ratio(tune * SEMITONE);
  settle(cowbell.metal, METALLIC + PAIR, BELLS, ratio, rate);
  FILTER::settle(cowbell.band, FILTER::BAND, CENTRE, WIDTH, FLAT, rate);
  ::envelop(cowbell, decay, rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::settle(
  Cowbell &cowbell, Float tune, Float decay, Float interval, Float tone,
  Whole rate) {
  const Float lower = METALLIC[PAIR];
  const Float pair[BELLS] = {lower, lower * PHASE::ratio(interval * SEMITONE)};
  settle(cowbell.metal, pair, BELLS, PHASE::ratio(tune * SEMITONE), rate);
  FILTER::settle(cowbell.band, FILTER::BAND, tone, WIDTH, FLAT, rate);
  ::envelop(cowbell, decay, rate);
}

void SOUND::PLUGINS::CORE::PERCUSSION::strike(
  Cowbell &cowbell, Float velocity) {
  strike(cowbell, struck(velocity), SHARP, RING);
}

void SOUND::PLUGINS::CORE::PERCUSSION::strike(
  Cowbell &cowbell, Float level, Float clank, Float ring) {
  cowbell.velocity = level;
  cowbell.bite = clank;
  cowbell.body = ring;
  ENVELOPE::strike(cowbell.hit, cowbell.clank, level);
  ENVELOPE::strike(cowbell.ring, cowbell.tail, level);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::tick(Cowbell &cowbell) -> Float {
  if (!sounding(cowbell)) return 0;
  const Float tone = FILTER::tick(cowbell.band, tick(cowbell.metal));
  const Float sharp = ENVELOPE::tick(cowbell.hit, cowbell.clank) * cowbell.bite;
  const Float ring = ENVELOPE::tick(cowbell.ring, cowbell.tail) * cowbell.body;
  return SHAPER::soft(tone * (sharp + ring) * cowbell.velocity * GAIN);
}
