// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

auto keyed(
  GATE::Effect &effect, AUDIO::PLUGIN::Sample *const *lanes,
  Whole frame) -> Float {
  Float heard = 0;
  for (Whole channel = 0; channel < effect.channels; ++channel) {
    const Float key =
      CORE::DYNAMICS::tick(effect.keys[channel], lanes[channel][frame]);
    const Float size = CORE::BLOCK::magnitude(key);
    if (size > heard) heard = size;
  }
  return heard;
}

void play(
  GATE::Effect &effect, AUDIO::PLUGIN::Sample *const *lanes, Whole frame) {
  const Float heard =
    CORE::DYNAMICS::tick(effect.sense, ::keyed(effect, lanes, frame));
  const Float open = heard > effect.threshold ? GATE::UNITY : 0;
  const Float held = CORE::DYNAMICS::tick(effect.hold, open);
  const Float swing = CORE::DYNAMICS::tick(effect.envelope, held);
  const Float level = effect.floor + (GATE::UNITY - effect.floor) * swing;
  for (Whole channel = 0; channel < effect.channels; ++channel)
    lanes[channel][frame] *= level;
}

}  // namespace

void SOUND::GATE::render(
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
