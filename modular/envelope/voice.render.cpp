// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::PLUGINS::ENVELOPE::render(
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
    follow(module, lanes, frame);
    const Float level = step(module) * module.rows[DEPTH];
    loop(module);
    for (Whole channel = 0; channel < module.channels; ++channel)
      CORE::BLOCK::lane(lanes, IN::GATE, channel, module.channels)[frame] =
        level;
    const Float size = CORE::BLOCK::magnitude(level);
    if (size > peak) peak = size;
  }
  CORE::BLOCK::rest(cursor, events, count, applied);
  module.allocator.notes[LEAD].sounding = CORE::ENVELOPE::sounding(module.gate);
  CORE::BLOCK::publish(module.meter, peak);
}
