// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void play(
  WAH::Effect &effect, AUDIO::PLUGIN::Sample *const *lanes, Whole frame) {
  const Float heard = CORE::DYNAMICS::tick(
    effect.detector, CORE::BLOCK::loudest(lanes, effect.channels, frame));
  WAH::sweep(effect, heard);
  for (Whole channel = 0; channel < effect.channels; ++channel) {
    Float &sample = lanes[channel][frame];
    sample = CORE::FILTER::tick(effect.strips[channel], sample) * effect.trim;
  }
}

}  // namespace

void SOUND::PLUGINS::WAH::render(
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
