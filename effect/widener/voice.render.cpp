// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

auto cascade(CORE::FILTER::Biquad *stages, Float in) -> Float {
  for (Whole stage = 0; stage < WIDENER::ORDER; ++stage)
    in = CORE::FILTER::tick(stages[stage], in);
  return in;
}

void widen(
  const WIDENER::Effect &effect, WIDENER::Strip &strip, Float &left,
  Float &right) {
  const Float mid = (left + right) * WIDENER::HALF * effect.mid;
  const Float side =
    ::cascade(strip.highs, (left - right) * WIDENER::HALF) * effect.side;
  left = mid + side;
  right = mid - side;
}

void play(
  WIDENER::Effect &effect, AUDIO::PLUGIN::Sample *const *lanes, Whole frame) {
  const Whole pairs = effect.strips.size();
  for (Whole pair = 0; pair < pairs; ++pair) {
    const Whole left = pair * WIDENER::SIDES + WIDENER::LEFT;
    const Whole right = pair * WIDENER::SIDES + WIDENER::RIGHT;
    ::widen(
      effect, effect.strips[pair], lanes[left][frame], lanes[right][frame]);
  }
  for (Whole lone = pairs * WIDENER::SIDES; lone < effect.channels; ++lone)
    lanes[lone][frame] *= effect.mid;
}

}  // namespace

void SOUND::WIDENER::render(
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
