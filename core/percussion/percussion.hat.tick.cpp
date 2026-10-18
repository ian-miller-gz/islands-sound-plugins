// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Whole PAIR = 2;
constexpr Float GAIN = 4.0f;

using Sounding = auto (*)(PERCUSSION::Metal &) -> Float;

constexpr Sounding BANKS[PERCUSSION::METAL::KINDS] = {
  [](PERCUSSION::Metal &metal) { return PERCUSSION::tick(metal); },
  [](PERCUSSION::Metal &metal) { return PERCUSSION::ring(metal); }};

}  // namespace

auto SOUND::PLUGINS::CORE::PERCUSSION::ring(Metal &metal) -> Float {
  const Whole pairs = metal.count / ::PAIR;
  if (pairs == 0) return 0;
  Float sum = 0;
  for (Whole at = 0; at < pairs; ++at) {
    OSCILLATOR::Oscillator *pair = metal.oscillators + at * ::PAIR;
    sum += OSCILLATOR::tick(pair[0], OSCILLATOR::PULSE) *
           OSCILLATOR::tick(pair[1], OSCILLATOR::PULSE);
  }
  return sum / Float(pairs);
}

auto SOUND::PLUGINS::CORE::PERCUSSION::tick(Hat &hat) -> Float {
  if (!sounding(hat)) return 0;
  const Float bank = ::BANKS[hat.setting.metal](hat.metal);
  const Float white = NOISE::tick(hat.white);
  const Float body =
    FILTER::tick(hat.band, bank + (white - bank) * hat.setting.noise);
  const Float shut = ENVELOPE::tick(hat.shut.gate, hat.envelopes[hat.opening]) *
                     hat.shut.velocity;
  const Float swing =
    ENVELOPE::tick(hat.open.gate, hat.envelopes[OPENING::OPEN]) *
    hat.open.velocity * tick(hat.choke);
  const Float out = FILTER::tick(hat.high, body * (shut + swing));
  return SHAPER::soft(out * ::GAIN);
}
