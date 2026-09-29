// SPDX-License-Identifier: AGPL-3.0-or-later
#include <algorithm>

#include "voice.internal.hpp"

namespace {
using namespace SOUND;

auto cascade(CORE::FILTER::Biquad *stages, Float in) -> Float {
  for (Whole stage = 0; stage < MULTIBAND::ORDER; ++stage)
    in = CORE::FILTER::tick(stages[stage], in);
  return in;
}

auto squeezed(
  MULTIBAND::Band &band, const Vector<MULTIBAND::Strip> &strips,
  Whole at) -> Float {
  Float heard = 0;
  for (const MULTIBAND::Strip &strip : strips)
    heard = std::max(heard, CORE::BLOCK::magnitude(strip.parts[at]));
  const Float level = CORE::DYNAMICS::tick(band.detector, heard);
  const Float cut =
    CORE::DYNAMICS::reduce(band.computer, CORE::DYNAMICS::decibels(level));
  return CORE::DYNAMICS::gain(cut) * band.makeup;
}

void play(
  MULTIBAND::Effect &effect, AUDIO::PLUGIN::Sample *const *lanes, Whole frame) {
  for (Whole channel = 0; channel < effect.channels; ++channel)
    MULTIBAND::divide(effect.strips[channel], lanes[channel][frame]);
  Float levels[MULTIBAND::BANDS];
  for (Whole at = 0; at < MULTIBAND::BANDS; ++at)
    levels[at] = ::squeezed(effect.bands[at], effect.strips, at);
  for (Whole channel = 0; channel < effect.channels; ++channel) {
    Float sum = 0;
    for (Whole at = 0; at < MULTIBAND::BANDS; ++at)
      sum += effect.strips[channel].parts[at] * levels[at];
    lanes[channel][frame] = sum;
  }
}

}  // namespace

void SOUND::MULTIBAND::divide(Strip &strip, Float in) {
  Split &first = strip.splits[FIRST];
  Split &last = strip.splits[LAST];
  const Float low = ::cascade(first.lows, in);
  const Float rest = ::cascade(first.highs, in);
  strip.parts[LOW] =
    ::cascade(strip.allpass.lows, low) + ::cascade(strip.allpass.highs, low);
  strip.parts[MID] = ::cascade(last.lows, rest);
  strip.parts[HIGH] = ::cascade(last.highs, rest);
}

void SOUND::MULTIBAND::render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &effect = *static_cast<Effect *>(instance);
  const auto steer = [&effect](const AUDIO::PLUGIN::Event &event) {
    apply(effect, event);
  };
  CORE::BLOCK::Cursor cursor;
  Float peak = 0;
  for (Whole frame = 0; frame < frames; ++frame) {
    CORE::BLOCK::due(cursor, events, count, frame, steer);
    ::play(effect, lanes, frame);
    const Float loud = CORE::BLOCK::loudest(lanes, effect.channels, frame);
    if (loud > peak) peak = loud;
  }
  CORE::BLOCK::rest(cursor, events, count, steer);
  CORE::BLOCK::publish(effect.meter, peak);
}
