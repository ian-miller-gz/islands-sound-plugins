// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

auto heard(
  const PINGPONG::Effect &effect, AUDIO::PLUGIN::Sample *const *lanes,
  Whole frame) -> Float {
  Float sum = 0;
  for (Whole channel = 0; channel < effect.channels; ++channel)
    sum += lanes[channel][frame];
  return sum / Float(effect.channels);
}

void place(
  const PINGPONG::Effect &effect, AUDIO::PLUGIN::Sample *const *lanes,
  Whole frame, const PINGPONG::Stereo &echo) {
  const Float sides[PINGPONG::SIDES] = {echo.left, echo.right};
  const Float middle = (echo.left + echo.right) * PINGPONG::HALF;
  for (Whole channel = 0; channel < effect.channels; ++channel) {
    const Float wet = effect.channels == PINGPONG::MONO
                        ? middle
                        : sides[channel % PINGPONG::SIDES];
    Float &sample = lanes[channel][frame];
    sample = sample * effect.dry + wet * effect.wet;
  }
}

}  // namespace

void SOUND::PINGPONG::render(
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
    ::place(effect, lanes, frame, bounce(effect, in));
    const Float loud = CORE::BLOCK::loudest(lanes, effect.channels, frame);
    if (loud > peak) peak = loud;
  }
  CORE::BLOCK::rest(cursor, events, count, steer);
  CORE::BLOCK::publish(effect.meter, peak);
}
