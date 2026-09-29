// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole ONE = 1;
constexpr Whole WHOLE = 0;
constexpr Whole FILLS = 4;
constexpr Float HIT = 1.0f;
constexpr Float ROLL = 0.7f;

constexpr Whole PARTS[RHYTHM::VOICES] = {
  WHOLE, WHOLE, CORE::PERCUSSION::OPENING::CLOSED,
  CORE::PERCUSSION::PLATE::RIDE};

auto filling(const RHYTHM::Box &box, Whole bar) -> Flag {
  return box.rows[RHYTHM::FILL] > 0 && bar % ::FILLS == ::FILLS - ::ONE;
}

void play(RHYTHM::Box &box, Whole edge) {
  const RHYTHM::Pattern &chosen =
    RHYTHM::pattern(Whole(box.rows[RHYTHM::PATTERN]));
  const Whole steps = RHYTHM::length(chosen);
  const Whole step = edge % steps;
  const Whole bar = edge / steps;
  for (Whole voice = 0; voice < RHYTHM::VOICES; ++voice)
    if (RHYTHM::struck(chosen, voice, step))
      RHYTHM::strike(box, voice, ::PARTS[voice], ::HIT);
  if (::filling(box, bar) && step + chosen.division >= steps)
    RHYTHM::strike(box, RHYTHM::SNARE, ::WHOLE, ::ROLL);
  if (step == 0 && bar > 0 && ::filling(box, bar - ::ONE))
    RHYTHM::strike(box, RHYTHM::CYMBAL, CORE::PERCUSSION::PLATE::CRASH, ::HIT);
}

using Tick = auto (*)(RHYTHM::Box &box) -> Float;

constexpr Tick TICKS[RHYTHM::VOICES] = {
  [](RHYTHM::Box &box) -> Float { return CORE::PERCUSSION::tick(box.kick); },
  [](RHYTHM::Box &box) -> Float { return CORE::PERCUSSION::tick(box.snare); },
  [](RHYTHM::Box &box) -> Float { return CORE::PERCUSSION::tick(box.hat); },
  [](RHYTHM::Box &box) -> Float { return CORE::PERCUSSION::tick(box.cymbal); }};

}  // namespace

void SOUND::PLUGINS::RHYTHM::pulse(Box &box) {
  CORE::CLOCK::Edge edge;
  if (CORE::CLOCK::advance(box.clock, ::ONE, &edge, ::ONE) == ::ONE)
    ::play(box, edge.step);
}

auto SOUND::PLUGINS::RHYTHM::mix(Box &box) -> Float {
  Float sum = 0;
  for (Whole voice = 0; voice < VOICES; ++voice)
    sum += ::TICKS[voice](box) * box.rows[LEVELS + voice];
  return CORE::SHAPER::soft(sum);
}
