// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "filter.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float ONE = 1.0f;
constexpr Float TWO = 2.0f;
constexpr Float SPEED = 35000.0f;
constexpr Float QUARTER = 4.0f;
constexpr Float OPENING = 0.55f;
constexpr Float REACH = 0.95f;
constexpr Float THIRD = 0.95f;
constexpr Float BRIGHT = 0.2f;
constexpr Float NARROWEST = 40.0f;
constexpr Float SPREAD = 0.04f;
constexpr Float SLOPE = -6.0f;
constexpr Float FIRST = 1.15f;
constexpr Float SECOND = 2.3f;
constexpr Float EFFORT = 12.0f;
constexpr Whole RINGED = 2;

struct Place {
  Float open;
  Float front;
};

struct Tube {
  Float length;
  Float pharynx;
  Float ring;
};

constexpr Place PLACES[] = {
  {1, 0.25f}, {0.45f, 0.8f}, {0, 1}, {0.4f, 0.05f}, {0, 0}};
static_assert(sizeof(PLACES) / sizeof(PLACES[0]) == FILTER::VOWELS);

constexpr Tube TUBES[] = {
  {18.5f, 0, 0.5f}, {17.5f, 0, 0.4f}, {15.5f, 0.1f, 0}, {14.5f, 0.15f, 0}};
static_assert(sizeof(TUBES) / sizeof(TUBES[0]) == FILTER::REGISTERS);

auto blend(Float from, Float to, Float part) -> Float {
  return from + (to - from) * part;
}

auto mixed(const Place &from, const Place &to, Float part) -> Place {
  return {
    ::blend(from.open, to.open, part), ::blend(from.front, to.front, part)};
}

auto mixed(const Tube &from, const Tube &to, Float part) -> Tube {
  return {
    ::blend(from.length, to.length, part),
    ::blend(from.pharynx, to.pharynx, part), ::blend(from.ring, to.ring, part)};
}

template <class Row, Whole Count>
auto spanned(const Row (&rows)[Count], Float at) -> Row {
  const Float last = Float(Count - 1);
  const Float held = at < 0 ? 0 : at > last ? last : at;
  const Whole from = Whole(held), to = from + 1 < Count ? from + 1 : from;
  return ::mixed(rows[from], rows[to], held - Float(from));
}

void centre(Float *centres, const Tube &tube, const Place &place) {
  for (Whole at = 0; at < FILTER::FORMANTS; ++at)
    centres[at] = (TWO * Float(at) + ONE) * SPEED / (QUARTER * tube.length);
  centres[0] *= OPENING + REACH * place.open;
  centres[1] *=
    (OPENING + REACH * place.front) * (ONE + tube.pharynx * place.front);
  centres[RINGED] *= THIRD + BRIGHT * place.front;
  Float mean = 0;
  for (Whole at = RINGED; at < FILTER::FORMANTS; ++at) mean += centres[at];
  mean /= Float(FILTER::FORMANTS - RINGED);
  for (Whole at = RINGED; at < FILTER::FORMANTS; ++at)
    centres[at] += tube.ring * (mean - centres[at]);
}

}  // namespace

void SOUND::PLUGINS::CORE::FILTER::aim(
  Tract &tract, Float voice, Float vowel, Float target, Float part) {
  const Place from = ::spanned(PLACES, vowel), to = ::spanned(PLACES, target);
  ::centre(
    tract.centres, ::spanned(TUBES, voice), ::mixed(from, to, bounded(part)));
  for (Whole at = 0; at < FORMANTS; ++at) {
    tract.widths[at] = NARROWEST + SPREAD * tract.centres[at];
    tract.levels[at] = SLOPE * std::log2(tract.centres[at] / tract.centres[0]);
  }
}

void SOUND::PLUGINS::CORE::FILTER::bend(
  Tract &tract, Float hertz, Float effort) {
  tract.centres[0] = std::fmax(tract.centres[0], FIRST * hertz);
  tract.centres[1] = std::fmax(tract.centres[1], SECOND * hertz);
  for (Whole at = RINGED; at < FORMANTS; ++at)
    tract.levels[at] += EFFORT * bounded(effort);
}
