// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto drive(const DISTORTION::Effect &effect, DISTORTION::Strip &strip, Float in)
  -> Float {
  const Float tight = CORE::FILTER::tick(strip.tight, in);
  const Float shaped =
    CORE::SHAPER::tick(strip.oversampler, effect.gain * tight, effect.curve);
  const Float held = CORE::SHAPER::tick(strip.blocker, shaped);
  return CORE::FILTER::tick(strip.tone, held);
}

void play(
  DISTORTION::Effect &effect, AUDIO::PLUGIN::Sample *const *lanes,
  Whole frame) {
  for (Whole channel = 0; channel < effect.channels; ++channel) {
    Float &sample = lanes[channel][frame];
    sample = effect.level * ::drive(effect, effect.strips[channel], sample);
  }
}

}  // namespace

void SOUND::PLUGINS::DISTORTION::render(
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
