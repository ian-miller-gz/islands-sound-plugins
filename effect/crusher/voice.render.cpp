// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto jittered(CRUSHER::Effect &effect) -> Float {
  const Float drawn = CORE::MODULATOR::draw(effect.seed);
  const Float stride = effect.stride * (CRUSHER::UNITY + effect.sway * drawn);
  return stride > CRUSHER::UNITY ? CRUSHER::UNITY : stride;
}

void play(
  CRUSHER::Effect &effect, AUDIO::PLUGIN::Sample *const *lanes, Whole frame) {
  const Float stride = ::jittered(effect);
  for (Whole channel = 0; channel < effect.channels; ++channel) {
    Float &sample = lanes[channel][frame];
    CORE::SHAPER::Crusher &crusher = effect.crushers[channel];
    crusher.stride = stride;
    const Float crushed = CORE::SHAPER::tick(crusher, sample);
    sample = effect.dry * sample + effect.wet * crushed;
  }
}

}  // namespace

void SOUND::PLUGINS::CRUSHER::render(
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
