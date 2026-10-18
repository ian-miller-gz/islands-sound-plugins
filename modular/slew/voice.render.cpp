// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void step(
  SLEW::Module &module, AUDIO::PLUGIN::Sample *const *lanes, Whole frame) {
  const Whole channels = module.channels;
  for (Whole channel = 0; channel < channels; ++channel) {
    const Float cv =
      CORE::BLOCK::lane(lanes, SLEW::CV, channel, channels)[frame];
    SLEW::tune(module, channel, cv);
    Float &sample =
      CORE::BLOCK::lane(lanes, SLEW::IN, channel, channels)[frame];
    sample = CORE::MODULATOR::tick(module.slews[channel], sample);
  }
}

}  // namespace

void SOUND::PLUGINS::SLEW::render(
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
    const Float size = CORE::BLOCK::loudest(lanes, module.channels, frame);
    if (size > peak) peak = size;
  }
  CORE::BLOCK::rest(cursor, events, count, applied);
  CORE::BLOCK::publish(module.meter, peak);
}
