// SPDX-License-Identifier: AGPL-3.0-or-later
#include <array>
#include <cmath>

#include "envelope.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float SLOWEST = 38.0f;
constexpr Float QUICKEN = 0.894924f;
constexpr Float STEP = 0.75f;
constexpr Float TEN = 10.0f;
constexpr Float TWENTY = 20.0f;
constexpr Float SINGLE = 1.0f;

constexpr auto sweep() -> std::array<Float, ENVELOPE::RATES> {
  std::array<Float, ENVELOPE::RATES> spans{};
  Float span = SLOWEST;
  for (Float &at : spans) {
    at = span;
    span *= QUICKEN;
  }
  return spans;
}

constexpr std::array<Float, ENVELOPE::RATES> SWEEPS = sweep();

constexpr Whole DECAYING = 1;

constexpr Whole NEXT[ENVELOPE::SEGMENTS] = {
  DECAYING, ENVELOPE::HOLD, ENVELOPE::HOLD, ENVELOPE::RESTED};

auto linear(Float level) -> Float { return level; }

auto exponential(Float level) -> Float {
  if (level <= 0) return 0;
  const Float decibels = (level - SINGLE) * ENVELOPE::TOP * STEP;
  return std::pow(TEN, decibels / TWENTY);
}

using Loudness = auto (*)(Float) -> Float;

constexpr Loudness LOUDNESS[ENVELOPE::CURVES] = {linear, exponential};

auto bounded(Float value) -> Float {
  return value < 0 ? 0 : value > ENVELOPE::TOP ? ENVELOPE::TOP : value;
}

}  // namespace

auto SOUND::PLUGINS::CORE::ENVELOPE::seconds(Float rate) -> Float {
  const Float place = ::bounded(rate);
  const Whole whole = Whole(place);
  if (whole + 1 >= RATES) return ::SWEEPS[RATES - 1];
  const Float part = place - Float(whole);
  return ::SWEEPS[whole] + (::SWEEPS[whole + 1] - ::SWEEPS[whole]) * part;
}

auto SOUND::PLUGINS::CORE::ENVELOPE::loudness(Float level, Whole curve)
  -> Float {
  return ::LOUDNESS[curve < CURVES ? curve : LINEAR](level);
}

void SOUND::PLUGINS::CORE::ENVELOPE::shape(Segments &segments, Whole rate) {
  for (Whole segment = 0; segment < SEGMENTS; ++segment) {
    const Float span = frames(seconds(segments.rates[segment]), rate);
    segments.steps[segment] = FULL / (span <= ::SINGLE ? ::SINGLE : span);
    segments.targets[segment] = ::bounded(segments.levels[segment]) / TOP;
  }
}

void SOUND::PLUGINS::CORE::ENVELOPE::strike(
  Walk &walk, const Segments &segments, Float velocity) {
  if (walk.segment >= RESTED) walk.level = segments.targets[SEGMENTS - 1];
  walk.segment = 0;
  walk.scale = weigh(velocity, segments.depth);
}

void SOUND::PLUGINS::CORE::ENVELOPE::lift(Walk &walk) {
  if (walk.segment < RESTED) walk.segment = SEGMENTS - 1;
}

auto SOUND::PLUGINS::CORE::ENVELOPE::sounding(const Walk &walk) -> Flag {
  return walk.segment < RESTED;
}

auto SOUND::PLUGINS::CORE::ENVELOPE::tick(Walk &walk, const Segments &segments)
  -> Float {
  if (walk.segment < RESTED) {
    const Float target = segments.targets[walk.segment];
    const Float step = segments.steps[walk.segment];
    if (walk.level < target)
      walk.level = walk.level + step < target ? walk.level + step : target;
    else
      walk.level = walk.level - step > target ? walk.level - step : target;
    if (walk.level == target) walk.segment = ::NEXT[walk.segment];
  }
  return loudness(walk.level, segments.curve) * walk.scale;
}
