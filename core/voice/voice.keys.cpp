// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

using Strike = void (*)(VOICE::Allocator &, Whole, Float);
using Lift = void (*)(VOICE::Allocator &, Whole);

constexpr Strike STRIKES[VOICE::MODES] = {
  VOICE::POLYPHONY::strike, VOICE::MONOPHONY::strike, VOICE::DUOPHONY::strike};
constexpr Lift LIFTS[VOICE::MODES] = {
  VOICE::POLYPHONY::lift, VOICE::MONOPHONY::lift, VOICE::DUOPHONY::lift};

auto mode(const VOICE::Allocator &allocator) -> Whole {
  return allocator.mode < VOICE::MODES ? allocator.mode : VOICE::POLY;
}

}  // namespace

void SOUND::PLUGINS::CORE::VOICE::strike(
  Allocator &allocator, Whole pitch, Float velocity) {
  if (pitch >= PHASE::PITCHES) return;
  allocator.lifts[pitch] = false;
  ::STRIKES[::mode(allocator)](allocator, pitch, velocity);
}

void SOUND::PLUGINS::CORE::VOICE::lift(Allocator &allocator, Whole pitch) {
  if (pitch >= PHASE::PITCHES) return;
  if (allocator.hold) {
    allocator.lifts[pitch] = true;
    return;
  }
  ::LIFTS[::mode(allocator)](allocator, pitch);
}

void SOUND::PLUGINS::CORE::VOICE::hold(Allocator &allocator, Flag held) {
  allocator.hold = held;
  if (held) return;
  for (Whole pitch = 0; pitch < PHASE::PITCHES; ++pitch) {
    if (!allocator.lifts[pitch]) continue;
    allocator.lifts[pitch] = false;
    ::LIFTS[::mode(allocator)](allocator, pitch);
  }
}
