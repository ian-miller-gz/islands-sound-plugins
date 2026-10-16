// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

auto SOUND::PLUGINS::SAMPLER::region(const Synth &synth, const Clip &clip)
  -> Region {
  const Whole count = frames(clip);
  if (count == 0) return {};
  const Float top = Float(count - 1);
  const Integer origin = Integer(synth.rows[START] / PERCENT * top);
  return {.origin = origin, .sign = 1, .length = top - Float(origin)};
}

auto SOUND::PLUGINS::SAMPLER::walk(Voice &voice) -> Flag {
  voice.along += voice.step;
  return voice.along <= voice.region.length;
}
