// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;
using namespace CORE::PERCUSSION;

constexpr Whole ONE = 1;
constexpr Whole WHOLE = 0;
constexpr Whole FILLS = 4;
constexpr Float HIT = 1.0f;
constexpr Float ROLL = 0.7f;
constexpr Float DIP = 0.4f;

constexpr Whole PARTS[RHYTHM::VOICES] = {
  WHOLE,       WHOLE, OPENING::CLOSED, PLATE::RIDE,
  HEIGHT::LOW, WHOLE, CLICK::RIM,      WHOLE};

struct Stroke {
  Whole voice;
  Whole part;
};

constexpr Stroke STROKES[] = {
  {RHYTHM::SNARE, WHOLE},
  {RHYTHM::TOM, HEIGHT::HIGH},
  {RHYTHM::TOM, HEIGHT::MID},
  {RHYTHM::TOM, HEIGHT::LOW}};

constexpr Whole STROKED = sizeof(STROKES) / sizeof(STROKES[0]);

auto filling(const RHYTHM::Box &box, Whole bar) -> Flag {
  return box.rows[RHYTHM::FILL] > 0 && bar % ::FILLS == ::FILLS - ::ONE;
}

auto weight(const RHYTHM::Box &box, const RHYTHM::Pattern &chosen, Whole step)
  -> Float {
  if (RHYTHM::accented(chosen, step)) return ::HIT;
  return ::HIT - box.rows[RHYTHM::ACCENT] * ::DIP;
}

void fill(RHYTHM::Box &box, Whole steps, Whole step, Float velocity) {
  const Whole left = steps - step;
  if (left > ::STROKED) return;
  const Stroke &stroke = ::STROKES[::STROKED - left];
  RHYTHM::queue(box, stroke.voice, stroke.part, velocity * ::ROLL);
}

void play(RHYTHM::Box &box, Whole edge) {
  const RHYTHM::Pattern &chosen =
    RHYTHM::pattern(Whole(box.rows[RHYTHM::PATTERN]));
  const Whole steps = RHYTHM::length(chosen);
  const Whole step = edge % steps;
  const Whole bar = edge / steps;
  const Float velocity = ::weight(box, chosen, step);
  for (Whole voice = 0; voice < RHYTHM::VOICES; ++voice)
    if (RHYTHM::struck(chosen, voice, step))
      RHYTHM::queue(box, voice, ::PARTS[voice], velocity);
  if (::filling(box, bar) && step + chosen.division >= steps)
    ::fill(box, steps, step, velocity);
  if (step == 0 && bar > 0 && ::filling(box, bar - ::ONE))
    RHYTHM::queue(box, RHYTHM::CYMBAL, PLATE::CRASH, ::HIT);
}

}  // namespace

void SOUND::PLUGINS::RHYTHM::pulse(Box &box) {
  drain(box);
  CORE::CLOCK::Edge edge;
  if (CORE::CLOCK::advance(box.clock, ::ONE, &edge, ::ONE) == ::ONE)
    ::play(box, edge.step);
}
