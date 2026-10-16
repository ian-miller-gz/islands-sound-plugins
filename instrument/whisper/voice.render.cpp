// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto voiced(WHISPER::Mouth &mouth) -> CORE::STEREO::Pair {
  if (CORE::BLOCK::due(mouth.stride, WHISPER::STRIDE)) WHISPER::steer(mouth);
  const CORE::STEREO::Pair out = WHISPER::breathe(mouth);
  return {out.left * mouth.gain, out.right * mouth.gain};
}

}  // namespace

void SOUND::PLUGINS::WHISPER::render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &mouth = *static_cast<Mouth *>(instance);
  const auto take = [&mouth](const AUDIO::PLUGIN::Event &event) {
    apply(mouth, event);
  };
  CORE::BLOCK::Cursor cursor;
  Float peak = 0;
  for (Whole frame = 0; frame < frames; ++frame) {
    CORE::BLOCK::due(cursor, events, count, frame, take);
    CORE::STEREO::pour(lanes, mouth.channels, frame, ::voiced(mouth));
    const Float size = CORE::BLOCK::loudest(lanes, mouth.channels, frame);
    if (size > peak) peak = size;
  }
  CORE::BLOCK::rest(cursor, events, count, take);
  CORE::BLOCK::publish(mouth.meter, peak);
}
