// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../block/block.hpp"

namespace SOUND::PLUGINS::CORE::STEREO {

struct Pair {
  Float left = 0;
  Float right = 0;
};

void pour(
  AUDIO::PLUGIN::Sample *const *lanes, Whole channels, Whole frame, Pair pair);

}  // namespace SOUND::PLUGINS::CORE::STEREO
