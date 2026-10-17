// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

void SOUND::PLUGINS::VOWEL::render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &singer = *static_cast<Singer *>(instance);
  const auto take = [&singer](const AUDIO::PLUGIN::Event &event) {
    apply(singer, event);
  };
  CORE::BLOCK::Cursor cursor;
  Float peak = 0;
  for (Whole frame = 0; frame < frames; ++frame) {
    CORE::BLOCK::due(cursor, events, count, frame, take);
    const CORE::STEREO::Pair sung = sing(singer);
    const CORE::STEREO::Pair pair = {
      sung.left * singer.gain, sung.right * singer.gain};
    CORE::STEREO::pour(lanes, singer.channels, frame, pair);
    const Float size = CORE::BLOCK::loudest(lanes, singer.channels, frame);
    if (size > peak) peak = size;
  }
  CORE::BLOCK::rest(cursor, events, count, take);
  CORE::BLOCK::publish(singer.meter, peak);
}
