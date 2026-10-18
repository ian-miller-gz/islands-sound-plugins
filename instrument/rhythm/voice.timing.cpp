// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float MILLISECONDS = 1000.0f;
constexpr Float UNIT = 1.0f;
constexpr Float HALF = 0.5f;

auto drawn(RHYTHM::Box &box) -> Float {
  return (CORE::NOISE::tick(box.white) + ::UNIT) * ::HALF;
}

void land(RHYTHM::Box &box, Whole voice) {
  RHYTHM::Pending &pending = box.pendings[voice];
  pending.left = 0;
  RHYTHM::strike(box, voice, pending.part, pending.velocity);
}

}  // namespace

void SOUND::PLUGINS::RHYTHM::queue(
  Box &box, Whole voice, Whole part, Float velocity) {
  if (voice >= VOICES) return;
  if (box.pendings[voice].left > 0) ::land(box, voice);
  const Float lowered = ::UNIT - box.rows[DYNAMICS] * ::drawn(box);
  const Float seconds = box.rows[TIMING] * ::drawn(box) / ::MILLISECONDS;
  const Whole delay = Whole(seconds * Float(box.rate));
  if (delay == 0) return strike(box, voice, part, velocity * lowered);
  box.pendings[voice] = {delay, part, velocity * lowered};
}

void SOUND::PLUGINS::RHYTHM::drain(Box &box) {
  for (Whole voice = 0; voice < VOICES; ++voice) {
    Pending &pending = box.pendings[voice];
    if (pending.left > 0 && --pending.left == 0) ::land(box, voice);
  }
}

void SOUND::PLUGINS::RHYTHM::rewind(Box &box) {
  CORE::NOISE::seed(box.white, CORE::NOISE::SEED);
  for (Pending &pending : box.pendings) pending = {};
}
