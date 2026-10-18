// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float SIZE = 1;
constexpr Float SECONDS = 1.5f;
constexpr Float DAMPING = 4000;
constexpr Float HALF = 0.5f;

}  // namespace

void SOUND::PLUGINS::DRAWBAR::build(Room &room, Whole rate) {
  CORE::REVERB::build(room.network, CORE::REVERB::FOUR, rate);
  CORE::REVERB::settle(room.network, ::SIZE, ::SECONDS, ::DAMPING);
}

auto SOUND::PLUGINS::DRAWBAR::tick(Room &room, Float wet, Pair dry) -> Pair {
  const Float mid = ::HALF * (dry.left + dry.right);
  const CORE::REVERB::Stereo tail = CORE::REVERB::tick(room.network, mid);
  return {dry.left + wet * tail.left, dry.right + wet * tail.right};
}
