// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float HALF = 0.5f;

void pour(
  AUDIO::PLUGIN::Sample *const *lanes, Whole channels, Whole frame,
  CHOIR::Stereo out) {
  const Float mid = (out.left + out.right) * HALF;
  if (channels < CHOIR::SIDES) {
    lanes[0][frame] = mid;
    return;
  }
  lanes[0][frame] = out.left;
  lanes[1][frame] = out.right;
  for (Whole channel = CHOIR::SIDES; channel < channels; ++channel)
    lanes[channel][frame] = mid;
}

auto voiced(CHOIR::Choir &choir) -> CHOIR::Stereo {
  if (choir.clock == 0) CHOIR::steer(choir);
  choir.clock = (choir.clock + 1) % CHOIR::CONTROL;
  const CHOIR::Stereo out = CHOIR::spread(choir, CHOIR::sing(choir));
  return {out.left * choir.gain, out.right * choir.gain};
}

}  // namespace

void SOUND::CHOIR::render(
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
    ::pour(lanes, choir.channels, frame, ::voiced(choir));
    const Float size = CORE::BLOCK::loudest(lanes, choir.channels, frame);
    if (size > peak) peak = size;
  }
  CORE::BLOCK::rest(cursor, events, count, take);
  CORE::BLOCK::publish(choir.meter, peak);
}
