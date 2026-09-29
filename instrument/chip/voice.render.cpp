// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::CHIP::render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &synth = *static_cast<Synth *>(instance);
  const auto land = [&synth](const AUDIO::PLUGIN::Event &event) {
    apply(synth, event);
  };
  CORE::BLOCK::Cursor cursor;
  Float peak = 0;
  for (Whole frame = 0; frame < frames; ++frame) {
    CORE::BLOCK::due(cursor, events, count, frame, land);
    const Float sample = sound(synth) * synth.rows[VOLUME];
    for (Whole channel = 0; channel < synth.channels; ++channel)
      lanes[channel][frame] = sample;
    const Float size = CORE::BLOCK::magnitude(sample);
    if (size > peak) peak = size;
  }
  CORE::BLOCK::rest(cursor, events, count, land);
  CORE::BLOCK::publish(synth.meter, CORE::BLOCK::clipped(peak));
}
