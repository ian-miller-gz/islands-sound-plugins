// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

auto bounded(Float value, Float ceiling) -> Float {
  return value < -ceiling ? -ceiling : value > ceiling ? ceiling : value;
}

auto needed(const LIMITER::Effect &effect, Float heard) -> Float {
  if (heard <= effect.ceiling) return 0;
  return LIMITER::UNITY - effect.ceiling / heard;
}

void play(
  LIMITER::Effect &effect, AUDIO::PLUGIN::Sample *const *lanes, Whole frame) {
  const Float heard =
    CORE::BLOCK::loudest(lanes, effect.channels, frame) * effect.gain;
  const Float held = CORE::DYNAMICS::tick(effect.hold, ::needed(effect, heard));
  const Float cut = CORE::DYNAMICS::tick(effect.detector, held);
  const Float level = effect.gain * (LIMITER::UNITY - cut);
  for (Whole channel = 0; channel < effect.channels; ++channel) {
    Float &sample = lanes[channel][frame];
    const Float late = CORE::DYNAMICS::tick(effect.rings[channel], sample);
    sample = ::bounded(late * level, effect.ceiling);
  }
}

}  // namespace

void SOUND::LIMITER::render(
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
