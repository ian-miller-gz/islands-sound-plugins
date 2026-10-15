// SPDX-License-Identifier: AGPL-3.0-or-later
#include <xmmintrin.h>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void denormals() { _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON); }

void publish(SUBTRACTIVE::Synth &synth, Float peak) {
  const Whole idle = synth.face.load(std::memory_order_relaxed) ^ 1u;
  synth.level[idle] = peak;
  synth.face.store(idle, std::memory_order_release);
}

}  // namespace

void SOUND::PLUGINS::SUBTRACTIVE::render(
  void *instance, AUDIO::PLUGIN::Sample *const *outputs, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  ::denormals();
  auto &synth = *static_cast<Synth *>(instance);
  Whole next = 0;
  Float peak = 0;
  for (Whole frame = 0; frame < frames; ++frame) {
    while (next < count && events[next].offset <= frame)
      apply(synth, events[next++]);
    const Float sample = sound(synth);
    for (Whole channel = 0; channel < synth.channels; ++channel)
      outputs[channel][frame] = sample;
    const Float magnitude = sample < 0 ? -sample : sample;
    if (magnitude > peak) peak = magnitude;
  }
  while (next < count) apply(synth, events[next++]);
  ::publish(synth, peak);
}
