// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto voiced(CHOIR::Choir &choir) -> CHOIR::Stereo {
  if (CORE::BLOCK::due(choir.stride, CHOIR::CONTROL)) CHOIR::steer(choir);
  const CHOIR::Stereo out = CHOIR::spread(choir, CHOIR::sing(choir));
  return {out.left * choir.gain, out.right * choir.gain};
}

}  // namespace

void SOUND::PLUGINS::CHOIR::render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &choir = *static_cast<Choir *>(instance);
  const auto take = [&choir](const AUDIO::PLUGIN::Event &event) {
    apply(choir, event);
  };
  CORE::BLOCK::Cursor cursor;
  Float peak = 0;
  for (Whole frame = 0; frame < frames; ++frame) {
    CORE::BLOCK::due(cursor, events, count, frame, take);
    CORE::STEREO::pour(lanes, choir.channels, frame, ::voiced(choir));
    const Float size = CORE::BLOCK::loudest(lanes, choir.channels, frame);
    if (size > peak) peak = size;
  }
  CORE::BLOCK::rest(cursor, events, count, take);
  CORE::BLOCK::publish(choir.meter, peak);
}
