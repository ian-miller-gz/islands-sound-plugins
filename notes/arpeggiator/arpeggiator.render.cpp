// SPDX-License-Identifier: AGPL-3.0-or-later
#include "arpeggiator.internal.hpp"

void SOUND::ARPEGGIATOR::render(
  void *instance, AUDIO::PLUGIN::Sample *const *, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &arpeggio = *static_cast<Arpeggio *>(instance);
  const auto applied = [&arpeggio](const AUDIO::PLUGIN::Event &event) {
    apply(arpeggio, event);
  };
  arpeggio.out.written = 0;
  CORE::BLOCK::Cursor cursor;
  for (Whole frame = 0; frame < frames; ++frame) {
    CORE::BLOCK::due(cursor, events, count, frame, applied);
    tick(arpeggio, frame);
  }
  CORE::BLOCK::rest(cursor, events, count, applied);
}
