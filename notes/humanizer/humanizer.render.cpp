// SPDX-License-Identifier: AGPL-3.0-or-later
#include "humanizer.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void flush(HUMANIZER::Humanizer &humanizer, Whole frame) {
  const Whole moment = humanizer.now + frame;
  Whole kept = 0;
  for (Whole at = 0; at < humanizer.queued; ++at) {
    const HUMANIZER::Pending pending = humanizer.queue[at];
    if (pending.due > moment) {
      humanizer.queue[kept++] = pending;
      continue;
    }
    HUMANIZER::send(humanizer, pending.event, frame);
  }
  humanizer.queued = kept;
}

}  // namespace

void SOUND::PLUGINS::HUMANIZER::render(
  void *instance, AUDIO::PLUGIN::Sample *const *, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &humanizer = *static_cast<Humanizer *>(instance);
  const auto applied = [&humanizer](const AUDIO::PLUGIN::Event &event) {
    apply(humanizer, event);
  };
  humanizer.out.written = 0;
  CORE::BLOCK::Cursor cursor;
  for (Whole frame = 0; frame < frames; ++frame) {
    CORE::BLOCK::due(cursor, events, count, frame, applied);
    if (humanizer.queued > 0) ::flush(humanizer, frame);
  }
  CORE::BLOCK::rest(cursor, events, count, applied);
  humanizer.now += frames;
}
