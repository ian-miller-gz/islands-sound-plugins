// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"
#include "../../core/shaper/shaper.hpp"

namespace {
using namespace SOUND;

auto drive(OVERDRIVE::Strip &strip, Float in, Float gain) -> Float {
  const Float mids = CORE::FILTER::tick(strip.hump, in);
  return in + CORE::SHAPER::soft(gain * mids);
}

void play(
  OVERDRIVE::Effect &effect, AUDIO::PLUGIN::Sample *const *lanes, Whole frame) {
  for (Whole channel = 0; channel < effect.channels; ++channel) {
    Float &sample = lanes[channel][frame];
    OVERDRIVE::Strip &strip = effect.strips[channel];
    const Float driven = ::drive(strip, sample, effect.gain);
    sample = effect.level * CORE::FILTER::tick(strip.tone, driven);
  }
}

}  // namespace

void SOUND::OVERDRIVE::render(
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
