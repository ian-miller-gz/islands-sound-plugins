// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

auto scattered(GRAIN::Strip &strip) -> Float {
  return GRAIN::HALF * (CORE::MODULATOR::draw(strip.seed) + GRAIN::UNITY);
}

auto heard(GRAIN::Effect &effect, GRAIN::Strip &strip, Float in) -> Float {
  const Float out = CORE::CLOUD::tick(strip.cloud, strip.line);
  const Float fed = effect.feedback * CORE::SHAPER::soft(out);
  CORE::LINE::write(strip.line, in + fed);
  return out;
}

void play(
  GRAIN::Effect &effect, AUDIO::PLUGIN::Sample *const *lanes, Whole frame) {
  effect.countdown -= GRAIN::UNITY;
  if (effect.countdown <= 0) {
    effect.countdown += effect.interval;
    GRAIN::sow(effect);
  }
  for (Whole channel = 0; channel < effect.channels; ++channel) {
    Float &sample = lanes[channel][frame];
    const Float out = ::heard(effect, effect.strips[channel], sample);
    sample = effect.dry * sample + effect.wet * out;
  }
}

}  // namespace

void SOUND::GRAIN::sow(Effect &effect) {
  CORE::CLOUD::Grain grain = effect.grain;
  for (Strip &strip : effect.strips) {
    const Float scatter = effect.spray * ::scattered(strip);
    grain.delay = CORE::LINE::NEAREST + effect.lift + scatter;
    CORE::CLOUD::start(strip.cloud, grain);
  }
}

void SOUND::GRAIN::render(
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
