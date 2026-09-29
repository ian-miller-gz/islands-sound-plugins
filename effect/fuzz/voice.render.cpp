// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

auto gated(FUZZ::Strip &strip, Float in) -> Float {
  const Float heard = CORE::DYNAMICS::tick(strip.gate, in);
  return heard >= FUZZ::FLOOR ? in : in * heard / FUZZ::FLOOR;
}

auto drive(const FUZZ::Effect &effect, FUZZ::Strip &strip, Float in) -> Float {
  const auto shape = [&effect](Float value) {
    return CORE::SHAPER::fold(value + effect.bias) - effect.rest;
  };
  const Float driven = effect.gain * ::gated(strip, in);
  const Float shaped = CORE::SHAPER::tick(strip.oversampler, driven, shape);
  const Float held = CORE::SHAPER::tick(strip.blocker, shaped);
  const Float low = CORE::FILTER::tick(strip.low, held);
  const Float high = CORE::FILTER::tick(strip.high, held);
  return effect.lows * low + effect.highs * high;
}

void play(
  FUZZ::Effect &effect, AUDIO::PLUGIN::Sample *const *lanes, Whole frame) {
  for (Whole channel = 0; channel < effect.channels; ++channel) {
    Float &sample = lanes[channel][frame];
    sample = effect.level * ::drive(effect, effect.strips[channel], sample);
  }
}

}  // namespace

void SOUND::FUZZ::render(
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
