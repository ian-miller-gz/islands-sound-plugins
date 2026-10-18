// SPDX-License-Identifier: AGPL-3.0-or-later
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float TWICE = 2;

auto bounded(Float value, Float low, Float high) -> Float {
  return value < low ? low : value > high ? high : value;
}

auto off(SAMPLER::Voice &voice) -> Flag {
  voice.along += voice.step;
  return voice.along <= voice.region.length;
}

auto forward(SAMPLER::Voice &voice) -> Flag {
  const Float start = Float(voice.region.start);
  const Float end = Float(voice.region.end);
  voice.along += voice.step;
  if (voice.along >= end) voice.along -= end - start;
  if (voice.along >= end) voice.along = start;
  return true;
}

auto pingpong(SAMPLER::Voice &voice) -> Flag {
  const Float start = Float(voice.region.start);
  const Float end = Float(voice.region.end);
  voice.along += voice.turned ? -voice.step : voice.step;
  if (!voice.turned && voice.along >= end) {
    voice.along = TWICE * end - voice.along;
    voice.turned = true;
  } else if (voice.turned && voice.along <= start) {
    voice.along = TWICE * start - voice.along;
    voice.turned = false;
  }
  const Float least = voice.turned ? start : 0;
  voice.along = ::bounded(voice.along, least, end);
  return true;
}

auto sustain(SAMPLER::Voice &voice) -> Flag {
  return voice.held ? forward(voice) : off(voice);
}

using Walker = auto (*)(SAMPLER::Voice &) -> Flag;

constexpr Walker WALKERS[] = {off, forward, pingpong, sustain};
static_assert(std::size(WALKERS) == std::size(SAMPLER::LOOPS));

}  // namespace

auto SOUND::PLUGINS::SAMPLER::walk(Voice &voice) -> Flag {
  return ::WALKERS[voice.region.walk](voice);
}
