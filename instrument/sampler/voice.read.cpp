// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

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

auto point(
  const SAMPLER::Synth &synth, const SAMPLER::Voice &voice,
  const Vector<Float> &line, Float along) -> Float {
  const SAMPLER::Region &region = voice.region;
  const SAMPLER::Blend blend = SAMPLER::blend(synth, region, along);
  Float out = blend.near * ::linear(line, region, along);
  if (blend.far <= 0) return out;
  const Float back = along - Float(region.end - region.start);
  return out + blend.far * ::linear(line, region, back);
}

}  // namespace

auto SOUND::PLUGINS::SAMPLER::read(
  const Synth &synth, const Voice &voice, Whole lane) -> Float {
  const Vector<Float> &line = synth.clips[voice.clip].lanes[lane];
  return ::point(synth, voice, line, voice.along);
}
