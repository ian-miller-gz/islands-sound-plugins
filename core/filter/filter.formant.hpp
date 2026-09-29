// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "filter.linear.hpp"

namespace SOUND::PLUGINS::CORE::FILTER {

enum Vowel : Whole { A, E, I, O, U, VOWELS };

constexpr Whole FORMANTS = 5;

struct Formant {
  Biquad bands[FORMANTS];
  Float gains[FORMANTS] = {};
};

void settle(Formant &formant, Float vowel, Float shift, Whole rate);
auto tick(Formant &formant, Float in) -> Float;

}  // namespace SOUND::PLUGINS::CORE::FILTER
