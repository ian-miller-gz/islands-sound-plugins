// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

auto heard(
  const PLATE::Effect &effect, AUDIO::PLUGIN::Sample *const *lanes,
  Whole frame) -> Float {
  Float sum = 0;
  for (Whole channel = 0; channel < effect.channels; ++channel)
    sum += lanes[channel][frame];
  return sum / Float(effect.channels);
}

void place(
  const PLATE::Effect &effect, AUDIO::PLUGIN::Sample *const *lanes, Whole frame,
  const CORE::REVERB::Stereo &tail) {
  const Float sides[CORE::REVERB::SIDES] = {tail.left, tail.right};
  const Float middle = (tail.left + tail.right) * PLATE::HALF;
  for (Whole channel = 0; channel < effect.channels; ++channel) {
    const Float wet = effect.channels == PLATE::MONO
                        ? middle
                        : sides[channel % CORE::REVERB::SIDES];
    Float &sample = lanes[channel][frame];
    sample = sample * effect.dry + wet * effect.wet;
  }
}

}  // namespace

void SOUND::PLATE::render(
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
    const Float late = CORE::REVERB::tick(effect.predelay, in);
    ::place(effect, lanes, frame, CORE::REVERB::tick(effect.plate, late));
    const Float loud = CORE::BLOCK::loudest(lanes, effect.channels, frame);
    if (loud > peak) peak = loud;
  }
  CORE::BLOCK::rest(cursor, events, count, steer);
  CORE::BLOCK::publish(effect.meter, peak);
}
