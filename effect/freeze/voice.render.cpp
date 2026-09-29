// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

auto looped(FREEZE::Effect &effect, FREEZE::Strip &strip, Flag frozen)
  -> Float {
  if (!frozen) return 0;
  return effect.level * CORE::CLOUD::tick(strip.cloud, strip.line);
}

auto frozen(FREEZE::Effect &effect, Float gate) -> Flag {
  if (!effect.held && gate <= FREEZE::QUIET) return false;
  effect.level *= effect.fade;
  effect.countdown -= FREEZE::UNITY;
  if (effect.countdown > 0) return true;
  effect.countdown += effect.interval;
  FREEZE::sow(effect);
  return true;
}

void play(
  FREEZE::Effect &effect, AUDIO::PLUGIN::Sample *const *lanes, Whole frame) {
  const Float target = effect.held ? FREEZE::UNITY : 0;
  const Float gate = CORE::MODULATOR::tick(effect.latch, target);
  const Flag still = ::frozen(effect, gate);
  const Float wet = effect.mix * gate;
  for (Whole channel = 0; channel < effect.channels; ++channel) {
    FREEZE::Strip &strip = effect.strips[channel];
    Float &sample = lanes[channel][frame];
    const Float loop = ::looped(effect, strip, still);
    if (!still) CORE::LINE::write(strip.line, sample);
    sample = (FREEZE::UNITY - wet) * sample + wet * loop;
  }
}

}  // namespace

void SOUND::FREEZE::sow(Effect &effect) {
  for (Strip &strip : effect.strips)
    CORE::CLOUD::start(strip.cloud, effect.grain);
}

void SOUND::FREEZE::render(
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
