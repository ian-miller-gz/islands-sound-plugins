// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto tap(const Vector<Float> &line, const SAMPLER::Region &region, Integer at)
  -> Float {
  const Integer last = Integer(line.size()) - 1;
  const Integer frame = region.origin + region.sign * at;
  return line[frame < 0 ? 0 : frame > last ? last : frame];
}

}  // namespace

auto SOUND::PLUGINS::SAMPLER::read(
  const Synth &synth, const Voice &voice, Whole lane) -> Float {
  const Vector<Float> &line = synth.clips[voice.clip].lanes[lane];
  Integer at = Integer(voice.along);
  if (Float(at) > voice.along) --at;
  const Float fraction = voice.along - Float(at);
  const Float here = ::tap(line, voice.region, at);
  const Float there = ::tap(line, voice.region, at + 1);
  return here + (there - here) * fraction;
}
