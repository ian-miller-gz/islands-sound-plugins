// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

auto heard(
  const ROOM::Effect &effect, AUDIO::PLUGIN::Sample *const *lanes,
  Whole frame) -> Float {
  Float sum = 0;
  for (Whole channel = 0; channel < effect.channels; ++channel)
    sum += lanes[channel][frame];
  return sum / Float(effect.channels);
}

void place(
  const ROOM::Effect &effect, AUDIO::PLUGIN::Sample *const *lanes, Whole frame,
  const CORE::REVERB::Stereo &tail) {
  const Float sides[CORE::REVERB::SIDES] = {tail.left, tail.right};
  const Float middle = (tail.left + tail.right) * ROOM::HALF;
  for (Whole channel = 0; channel < effect.channels; ++channel) {
    const Float wet = effect.channels == ROOM::MONO
                        ? middle
                        : sides[channel % CORE::REVERB::SIDES];
    Float &sample = lanes[channel][frame];
    sample = sample * effect.dry + wet * effect.wet;
  }
}

auto sound(ROOM::Effect &effect, Float in) -> CORE::REVERB::Stereo {
  const auto early = CORE::REVERB::tick(effect.reflections, in);
  const Float onset = (early.left + early.right) * ROOM::HALF;
  const auto late = CORE::REVERB::tick(effect.network, onset);
  return {
    early.left * ROOM::EARLY + late.left,
    early.right * ROOM::EARLY + late.right};
}

}  // namespace

void SOUND::ROOM::render(
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
    const Float in = ::heard(effect, lanes, frame);
    ::place(effect, lanes, frame, ::sound(effect, in));
    const Float loud = CORE::BLOCK::loudest(lanes, effect.channels, frame);
    if (loud > peak) peak = loud;
  }
  CORE::BLOCK::rest(cursor, events, count, steer);
  CORE::BLOCK::publish(effect.meter, peak);
}
