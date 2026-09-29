// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void play(
  RINGMOD::Effect &effect, AUDIO::PLUGIN::Sample *const *lanes, Whole frame) {
  const Float carrier = RINGMOD::tick(effect);
  const Float blend = effect.dry + effect.wet * carrier;
  for (Whole channel = 0; channel < effect.channels; ++channel)
    lanes[channel][frame] *= blend;
}

}  // namespace

void SOUND::PLUGINS::RINGMOD::render(
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
