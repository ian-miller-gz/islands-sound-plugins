// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto tape(const SATURATOR::Effect &effect, SATURATOR::Strip &strip, Float in)
  -> Float {
  const Float driven = effect.gain * in + effect.bias;
  const Float shaped = CORE::SHAPER::soft(driven) - effect.rest;
  const Float held = CORE::SHAPER::tick(strip.blocker, effect.trim * shaped);
  const Float hissed = held + effect.hiss * CORE::NOISE::tick(strip.hiss);
  const Float rolled = CORE::FILTER::tick(strip.low, hissed);
  return CORE::FILTER::tick(strip.high, rolled);
}

void play(
  SATURATOR::Effect &effect, AUDIO::PLUGIN::Sample *const *lanes, Whole frame) {
  for (Whole channel = 0; channel < effect.channels; ++channel) {
    Float &sample = lanes[channel][frame];
    const Float wet = ::tape(effect, effect.strips[channel], sample);
    sample = effect.dry * sample + effect.wet * wet;
  }
}

}  // namespace

void SOUND::PLUGINS::SATURATOR::render(
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
