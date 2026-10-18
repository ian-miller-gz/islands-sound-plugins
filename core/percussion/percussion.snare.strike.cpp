// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"
#include "../dynamics/dynamics.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

using PERCUSSION::SNARE::STROKES;

constexpr Float UPPER = 0.6f;
constexpr Float FULL = 1.0f;
constexpr Float THIRD = 1.0f / 3.0f;
constexpr Float MUTE = 0;
constexpr Float EVEN = 0;
constexpr Float RIMSHOT = 3.0f;

struct Trait {
  Float length;
  Float rattle;
  Float ping;
  Float click;
  Float decibels;
};

constexpr Trait TRAITS[STROKES] = {
  {FULL, FULL, MUTE, MUTE, EVEN},
  {THIRD, MUTE, MUTE, FULL, EVEN},
  {FULL, FULL, FULL, MUTE, RIMSHOT}};

auto traced(Whole stroke) -> const Trait & {
  return TRAITS[stroke < STROKES ? stroke : PERCUSSION::SNARE::HEAD];
}

void touch(PERCUSSION::Snare &snare, Float level) {
  const PERCUSSION::SNARE::Setting &setting = snare.setting;
  const Float tone = PERCUSSION::touched(setting.tone, level, setting.touch);
  FILTER::settle(
    snare.band, FILTER::BAND, tone, PERCUSSION::SNARE::WIDTH,
    PERCUSSION::SNARE::FLAT, snare.rate);
}

}  // namespace

void SOUND::PLUGINS::CORE::PERCUSSION::strike(Snare &snare, Float velocity) {
  strike(snare, SNARE::HEAD, struck(velocity));
}

void SOUND::PLUGINS::CORE::PERCUSSION::strike(
  Snare &snare, Whole stroke, Float level) {
  const Trait &trait = ::traced(stroke);
  const Float hit = struck(level) * DYNAMICS::gain(trait.decibels);
  ::touch(snare, struck(level));
  ring(snare, trait.length);
  strike(snare.low, snare.low.base, hit);
  strike(snare.high, snare.high.base, hit * UPPER);
  for (Whole at = 0; at < SNARE::MODES; ++at)
    strike(snare.modes[at], snare.modes[at].base, hit * SNARE::WEIGHTS[at]);
  strike(snare.ping, hit * trait.ping);
  strike(snare.click, hit * trait.click);
  snare.level = hit * trait.rattle;
  ENVELOPE::strike(snare.gate, snare.rattle, snare.level);
  snare.holding = snare.hold;
}
