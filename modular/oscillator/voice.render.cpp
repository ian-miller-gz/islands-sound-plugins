// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto saw(OSCILLATOR::Module &module, Float since) -> Float {
  return CORE::OSCILLATOR::tick(module.sync, since, CORE::OSCILLATOR::SAW);
}

auto pulse(OSCILLATOR::Module &module, Float since) -> Float {
  return CORE::OSCILLATOR::tick(module.sync, since, CORE::OSCILLATOR::PULSE);
}

auto triangle(OSCILLATOR::Module &module, Float since) -> Float {
  return CORE::OSCILLATOR::tick(module.sync, since, CORE::OSCILLATOR::TRIANGLE);
}

auto sine(OSCILLATOR::Module &module, Float since) -> Float {
  CORE::OSCILLATOR::Oscillator &oscillator = module.sync.slave;
  const Float value = CORE::OSCILLATOR::sine(module.table, oscillator.phase);
  oscillator.phase += oscillator.step;
  if (since >= 0) CORE::OSCILLATOR::reset(oscillator, 0);
  return value;
}

auto super(OSCILLATOR::Module &module, Float) -> Float {
  return CORE::OSCILLATOR::tick(module.super);
}

using Shape = auto (*)(OSCILLATOR::Module &, Float) -> Float;

constexpr Shape SHAPES[OSCILLATOR::WAVES] = {saw, pulse, triangle, sine, super};

auto sound(OSCILLATOR::Module &module, const OSCILLATOR::Ins &ins) -> Float {
  if (!OSCILLATOR::playing(module)) return 0;
  const Float *rows = module.rows;
  const Float pitch = OSCILLATOR::pitched(module, ins);
  const Flag moved = pitch != module.pitch || ins.fm != module.fm;
  if (module.stale || moved) OSCILLATOR::tune(module, pitch, ins.fm);
  const Whole wave = Whole(rows[OSCILLATOR::WAVE]);
  const Shape shape = SHAPES[wave < OSCILLATOR::WAVES ? wave : OSCILLATOR::SAW];
  return shape(module, ins.since) * rows[OSCILLATOR::GAIN];
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
    const Ins ins = read(module, lanes, frame);
    const Float sample = ::sound(module, ins);
    for (Whole channel = 0; channel < module.channels; ++channel)
      CORE::BLOCK::lane(lanes, IN::PITCH, channel, module.channels)[frame] =
        sample;
    const Float size = CORE::BLOCK::magnitude(sample);
    if (size > peak) peak = size;
  }
  CORE::BLOCK::rest(cursor, events, count, applied);
  CORE::BLOCK::publish(module.meter, peak);
}
