// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto delay(ECHO::Effect &effect) -> Float {
  const Float wobble = CORE::MODULATOR::tick(effect.wow);
  const Float warble = CORE::MODULATOR::tick(effect.flutter);
  const Float held = CORE::MODULATOR::tick(effect.time, effect.frames);
  return held + effect.reach * (wobble + warble);
}

void play(
  ECHO::Effect &effect, AUDIO::PLUGIN::Sample *const *lanes, Whole frame) {
  const Float at = ::delay(effect);
  for (Whole channel = 0; channel < effect.channels; ++channel) {
    Float &sample = lanes[channel][frame];
    const Float out =
      ECHO::tick(effect.tracks[channel], sample, at, effect.drive);
    sample = sample * effect.dry + out * effect.wet;
  }
}

}  // namespace

void SOUND::PLUGINS::ECHO::render(
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
