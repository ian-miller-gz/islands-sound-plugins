// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <type_traits>

#include "percussion.circuit.hpp"
#include "../stereo/stereo.hpp"

namespace SOUND::PLUGINS::CORE::PERCUSSION {

template <class Value>
auto write(
  AUDIO::PLUGIN::Sample *const *lanes, Whole channels, Whole frame,
  Value value) -> Float {
  if constexpr (std::is_same_v<Value, STEREO::Pair>) {
    const STEREO::Pair pair = {
      BLOCK::clipped(value.left), BLOCK::clipped(value.right)};
    STEREO::pour(lanes, channels, frame, pair);
    const Float left = BLOCK::magnitude(pair.left);
    const Float right = BLOCK::magnitude(pair.right);
    return left > right ? left : right;
  } else {
    const Float sample = BLOCK::clipped(value);
    for (Whole channel = 0; channel < channels; ++channel)
      lanes[channel][frame] = sample;
    return BLOCK::magnitude(sample);
  }
}

template <class Sound, class Apply>
auto play(
  AUDIO::PLUGIN::Sample *const *lanes, Whole channels, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count, Sound &&sound,
  Apply &&apply) -> Float {
  BLOCK::Cursor cursor;
  Float peak = 0;
  for (Whole frame = 0; frame < frames; ++frame) {
    BLOCK::due(cursor, events, count, frame, apply);
    const Float size = write(lanes, channels, frame, sound());
    if (size > peak) peak = size;
  }
  BLOCK::rest(cursor, events, count, apply);
  return peak;
}

}  // namespace SOUND::PLUGINS::CORE::PERCUSSION
