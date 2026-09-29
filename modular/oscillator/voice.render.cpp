// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto saw(OSCILLATOR::Module &module) -> Float {
  return CORE::OSCILLATOR::tick(module.oscillator, CORE::OSCILLATOR::SAW);
}

auto pulse(OSCILLATOR::Module &module) -> Float {
  return CORE::OSCILLATOR::tick(module.oscillator, CORE::OSCILLATOR::PULSE);
}

auto triangle(OSCILLATOR::Module &module) -> Float {
  return CORE::OSCILLATOR::tick(module.oscillator, CORE::OSCILLATOR::TRIANGLE);
}

auto sine(OSCILLATOR::Module &module) -> Float {
  CORE::OSCILLATOR::Oscillator &oscillator = module.oscillator;
  const Float value = CORE::OSCILLATOR::sine(module.table, oscillator.phase);
  oscillator.phase += oscillator.step;
  return value;
}

auto super(OSCILLATOR::Module &module) -> Float {
  return CORE::OSCILLATOR::tick(module.super);
}

using Shape = auto (*)(OSCILLATOR::Module &) -> Float;

constexpr Shape SHAPES[OSCILLATOR::WAVES] = {saw, pulse, triangle, sine, super};

auto sound(OSCILLATOR::Module &module) -> Float {
  if (!module.sounding) return 0;
  const Float *rows = module.rows;
  CORE::VOICE::Note &lead = module.allocator.notes[0];
  const Float glided = CORE::VOICE::tick(lead, module.allocator.glide);
  const Float pitch = glided + rows[OSCILLATOR::COARSE] +
                      rows[OSCILLATOR::FINE] / OSCILLATOR::CENTS;
  if (module.stale || pitch != module.pitch) OSCILLATOR::tune(module, pitch);
  const Whole wave = Whole(rows[OSCILLATOR::WAVE]);
  return SHAPES[wave < OSCILLATOR::WAVES ? wave : OSCILLATOR::SAW](module) *
         rows[OSCILLATOR::GAIN];
}

}  // namespace

void SOUND::PLUGINS::OSCILLATOR::render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &module = *static_cast<Module *>(instance);
  const auto applied = [&module](const AUDIO::PLUGIN::Event &event) {
    apply(module, event);
  };
  CORE::BLOCK::Cursor cursor;
  Float peak = 0;
  for (Whole frame = 0; frame < frames; ++frame) {
    CORE::BLOCK::due(cursor, events, count, frame, applied);
    const Float sample = ::sound(module);
    for (Whole channel = 0; channel < module.channels; ++channel)
      lanes[channel][frame] = sample;
    const Float size = CORE::BLOCK::magnitude(sample);
    if (size > peak) peak = size;
  }
  CORE::BLOCK::rest(cursor, events, count, applied);
  CORE::BLOCK::publish(module.meter, peak);
}
