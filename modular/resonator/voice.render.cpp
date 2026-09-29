// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void step(
  RESONATOR::Module &module, AUDIO::PLUGIN::Sample *const *lanes, Whole frame) {
  const Float *rows = module.rows;
  const Float swing =
    CORE::MODULATOR::tick(module.lfo) * rows[RESONATOR::SWEEP];
  if (module.countdown == 0) {
    const Whole channels = module.channels;
    for (Whole channel = 0; channel < channels; ++channel) {
      const Float cv = lanes[RESONATOR::CV * channels + channel][frame];
      RESONATOR::sweep(module, channel, swing + cv * rows[RESONATOR::DEPTH]);
    }
    module.countdown = RESONATOR::STRIDE;
  }
  --module.countdown;
}

}  // namespace

void SOUND::PLUGINS::RESONATOR::render(
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
    ::step(module, lanes, frame);
    for (Whole channel = 0; channel < module.channels; ++channel) {
      Float &sample = lanes[channel][frame];
      sample = CORE::FILTER::tick(module.filters[channel], sample);
    }
    const Float size = CORE::BLOCK::loudest(lanes, module.channels, frame);
    if (size > peak) peak = size;
  }
  CORE::BLOCK::rest(cursor, events, count, applied);
  CORE::BLOCK::publish(module.meter, peak);
}
