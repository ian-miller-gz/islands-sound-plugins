// SPDX-License-Identifier: AGPL-3.0-or-later
#include <iterator>

#include "voice.internal.hpp"
#include "../../core/oscillator/oscillator.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float THRESHOLD = 1;
constexpr Float HALF = 0.5f;
constexpr Whole POINTS = 4;

auto tap(const Vector<Float> &line, const SAMPLER::Region &region, Integer at)
  -> Float {
  if (region.wraps && at >= region.end) at -= region.end - region.start;
  const Integer last = Integer(line.size()) - 1;
  const Integer frame = region.origin + region.sign * at;
  return line[frame < 0 ? 0 : frame > last ? last : frame];
}

auto floored(Float along) -> Integer {
  const Integer at = Integer(along);
  return Float(at) > along ? at - 1 : at;
}

auto linear(
  const Vector<Float> &line, const SAMPLER::Region &region,
  Float along) -> Float {
  const Integer at = ::floored(along);
  const Float here = ::tap(line, region, at);
  const Float there = ::tap(line, region, at + 1);
  return here + (there - here) * (along - Float(at));
}

auto cubic(
  const Vector<Float> &line, const SAMPLER::Region &region,
  Float along) -> Float {
  const Integer at = ::floored(along);
  Float points[POINTS];
  for (Whole point = 0; point < POINTS; ++point)
    points[point] = ::tap(line, region, at + Integer(point) - 1);
  return CORE::OSCILLATOR::cubic(points, along - Float(at));
}

using Raw = auto (*)(const Vector<Float> &, const SAMPLER::Region &, Float)
  -> Float;

struct Reader {
  Raw raw;
  Flag doubled;
  Whole fallback;
};

constexpr Reader READERS[] = {
  {linear, false, 0}, {cubic, false, 1}, {cubic, true, 1}};
static_assert(std::size(READERS) == std::size(SAMPLER::QUALITIES));

auto point(
  const SAMPLER::Synth &synth, const SAMPLER::Voice &voice,
  const Vector<Float> &line, Float along) -> Float {
  const SAMPLER::Region &region = voice.region;
  const Raw raw = READERS[voice.reader].raw;
  const SAMPLER::Blend blend = SAMPLER::blend(synth, region, along);
  const Float out = blend.near * raw(line, region, along);
  if (blend.far <= 0) return out;
  const Float back = along - Float(region.end - region.start);
  return out + blend.far * raw(line, region, back);
}

}  // namespace

auto SOUND::PLUGINS::SAMPLER::reader(const Synth &synth, Float step) -> Whole {
  const Whole quality = Whole(synth.rows[QUALITY]);
  return step > ::THRESHOLD ? quality : ::READERS[quality].fallback;
}

auto SOUND::PLUGINS::SAMPLER::read(const Synth &synth, Voice &voice, Whole lane)
  -> Float {
  const Vector<Float> &line = synth.clips[voice.clip].lanes[lane];
  const Float early = ::point(synth, voice, line, voice.along);
  if (!::READERS[voice.reader].doubled) return early;
  const Float heading = voice.turned ? -voice.step : voice.step;
  const Float late = ::point(synth, voice, line, voice.along + heading * HALF);
  return CORE::SHAPER::down(voice.downs[lane], {early, late});
}
