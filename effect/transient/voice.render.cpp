// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto lead(Float over, Float under) -> Float {
  if (over <= under || over <= TRANSIENT::QUIET) return 0;
  return TRANSIENT::UNITY - under / over;
}

void play(
  TRANSIENT::Effect &effect, AUDIO::PLUGIN::Sample *const *lanes, Whole frame) {
  const Float heard = CORE::BLOCK::loudest(lanes, effect.channels, frame);
  const Float quick = CORE::DYNAMICS::tick(effect.quick, heard);
  const Float slow = CORE::DYNAMICS::tick(effect.slow, heard);
  const Float boost = effect.rows[TRANSIENT::ATTACK] * ::lead(quick, slow) +
                      effect.rows[TRANSIENT::SUSTAIN] * ::lead(slow, quick);
  const Float level = CORE::DYNAMICS::gain(boost);
  for (Whole channel = 0; channel < effect.channels; ++channel)
    lanes[channel][frame] *= level;
}

}  // namespace

void SOUND::PLUGINS::TRANSIENT::render(
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
