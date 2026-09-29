// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::PLUGINS::STEPS::render(
  void *instance, AUDIO::PLUGIN::Sample *const *, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &sequencer = *static_cast<Sequencer *>(instance);
  const auto applied = [&sequencer](const AUDIO::PLUGIN::Event &event) {
    apply(sequencer, event);
  };
  sequencer.written = 0;
  CORE::BLOCK::Cursor cursor;
  for (Whole frame = 0; frame < frames; ++frame) {
    CORE::BLOCK::due(cursor, events, count, frame, applied);
    tick(sequencer, frame);
  }
  CORE::BLOCK::rest(cursor, events, count, applied);
}
