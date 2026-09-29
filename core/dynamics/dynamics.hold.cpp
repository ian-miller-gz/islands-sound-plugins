// SPDX-License-Identifier: AGPL-3.0-or-later
#include "dynamics.hpp"

void SOUND::PLUGINS::CORE::DYNAMICS::settle(Hold &hold, Whole rate) {
  const Float frames = hold.time * Float(rate);
  hold.length = frames <= 0 ? 0 : Whole(frames);
  if (hold.left > hold.length) hold.left = hold.length;
}

auto SOUND::PLUGINS::CORE::DYNAMICS::tick(Hold &hold, Float in) -> Float {
  if (in >= hold.value) {
    hold.value = in;
    hold.left = hold.length;
  } else if (hold.left > 0) {
    --hold.left;
  } else {
    hold.value = in;
  }
  return hold.value;
}
