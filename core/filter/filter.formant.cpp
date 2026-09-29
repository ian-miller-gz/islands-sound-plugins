// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "filter.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float TEN = 10.0f;
constexpr Float DECIBELS = 20.0f;

struct Shape {
  Float centres[FILTER::FORMANTS];
  Float levels[FILTER::FORMANTS];
  Float widths[FILTER::FORMANTS];
};

constexpr Shape SHAPES[] = {
  {{650, 1080, 2650, 2900, 3250},
   {0, -6, -7, -8, -22},
   {80, 90, 120, 130, 140}},
  {{400, 1700, 2600, 3200, 3580},
   {0, -14, -12, -14, -20},
   {70, 80, 100, 120, 120}},
  {{290, 1870, 2800, 3250, 3540},
   {0, -15, -18, -20, -30},
   {40, 90, 100, 120, 120}},
  {{400, 800, 2600, 2800, 3000},
   {0, -10, -12, -12, -26},
   {40, 80, 100, 120, 120}},
  {{350, 600, 2700, 2900, 3300},
   {0, -20, -17, -14, -26},
   {40, 60, 100, 120, 120}}};
static_assert(sizeof(SHAPES) / sizeof(SHAPES[0]) == FILTER::VOWELS);

auto blend(Float from, Float to, Float part) -> Float {
  return from + (to - from) * part;
}

}  // namespace

void SOUND::PLUGINS::CORE::FILTER::settle(
  Formant &formant, Float vowel, Float shift, Whole rate) {
  const Float last = Float(U);
  const Float place = vowel < 0 ? 0 : vowel > last ? last : vowel;
  const Whole from = Whole(place);
  const Whole to = from < U ? from + 1 : U;
  const Float part = place - Float(from);
  const Shape &first = SHAPES[from], &second = SHAPES[to];
  for (Whole band = 0; band < FORMANTS; ++band) {
    const Float centre =
      ::blend(first.centres[band], second.centres[band], part);
    const Float width = ::blend(first.widths[band], second.widths[band], part);
    const Float level = ::blend(first.levels[band], second.levels[band], part);
    settle(formant.bands[band], BAND, centre * shift, centre / width, 0, rate);
    formant.gains[band] = std::pow(TEN, level / DECIBELS);
  }
}

auto SOUND::PLUGINS::CORE::FILTER::tick(Formant &formant, Float in) -> Float {
  Float sum = 0;
  for (Whole band = 0; band < FORMANTS; ++band)
    sum += formant.gains[band] * tick(formant.bands[band], in);
  return sum;
}
