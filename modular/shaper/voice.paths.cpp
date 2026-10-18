// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Shape = auto (*)(SHAPER::Stage &, Float) -> Float;
using Sampler = auto (*)(SHAPER::Stage &, Shape, Float) -> Float;

struct Path {
  Shape shape;
  Flag rated;
};

auto soft(SHAPER::Stage &, Float in) -> Float { return CORE::SHAPER::soft(in); }

auto hard(SHAPER::Stage &stage, Float in) -> Float {
  return CORE::SHAPER::tick(
    stage.hard, in, CORE::SHAPER::hard, CORE::SHAPER::AREA::hard);
}

auto tube(SHAPER::Stage &, Float in) -> Float { return CORE::SHAPER::tube(in); }

auto fold(SHAPER::Stage &stage, Float in) -> Float {
  return CORE::SHAPER::tick(
    stage.fold, in, CORE::SHAPER::fold, CORE::SHAPER::AREA::fold);
}

auto crush(SHAPER::Stage &stage, Float in) -> Float {
  return CORE::SHAPER::hard(CORE::SHAPER::tick(stage.crusher, in));
}

auto chebyshev(SHAPER::Stage &stage, Float in) -> Float {
  return CORE::SHAPER::tick(stage.chebyshev, CORE::SHAPER::hard(in));
}

auto oversampled(
  CORE::SHAPER::Oversampler &oversampler, SHAPER::Stage &stage, Shape shape,
  Float in) -> Float {
  const auto shaped = [&stage, shape](Float sample) {
    return shape(stage, sample);
  };
  return CORE::SHAPER::tick(oversampler, in, shaped);
}

auto single(SHAPER::Stage &stage, Shape shape, Float in) -> Float {
  return shape(stage, in);
}

auto twice(SHAPER::Stage &stage, Shape shape, Float in) -> Float {
  return oversampled(stage.oversampler, stage, shape, in);
}

auto quad(SHAPER::Stage &stage, Shape shape, Float in) -> Float {
  const auto inner = [&stage, shape](Float sample) {
    return oversampled(stage.inner, stage, shape, sample);
  };
  return CORE::SHAPER::tick(stage.oversampler, in, inner);
}

constexpr Path PATHS[] = {{soft, true}, {hard, true},   {tube, true},
                          {fold, true}, {crush, false}, {chebyshev, true}};
static_assert(std::size(PATHS) == SHAPER::CURVES);

constexpr Sampler SAMPLERS[] = {single, twice, quad};
static_assert(std::size(SAMPLERS) == SHAPER::QUALITIES);

}  // namespace

auto SOUND::PLUGINS::SHAPER::curved(
  const Module &module, Stage &stage, Float in) -> Float {
  const Whole curve = Whole(module.rows[CURVE]);
  const Whole quality = Whole(module.rows[QUALITY]);
  const ::Path &path = ::PATHS[curve < CURVES ? curve : SOFT];
  const Whole factor = path.rated && quality < QUALITIES ? quality : SINGLE;
  return ::SAMPLERS[factor](stage, path.shape, in);
}
