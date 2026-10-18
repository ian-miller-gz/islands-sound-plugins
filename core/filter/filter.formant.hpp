// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "filter.linear.hpp"

namespace SOUND::PLUGINS::CORE::FILTER {

enum Vowel : Whole { A, E, I, O, U, VOWELS };
enum Register : Whole { BASS, TENOR, ALTO, SOPRANO, REGISTERS };

constexpr Whole FORMANTS = 5;

struct Tract {
  Float centres[FORMANTS] = {};
  Float widths[FORMANTS] = {};
  Float levels[FORMANTS] = {};
};

struct Formant {
  Biquad bands[FORMANTS];
  Float gains[FORMANTS] = {};
  Float settled[FORMANTS] = {};
  Tract tract;
  Float breadth = 1;
  Float applied = 1;
};

void aim(Tract &tract, Float voice, Float vowel, Float target, Float part);
void bend(Tract &tract, Float hertz, Float effort);
auto glide(
  Formant &formant, const Tract &tract, Float pole, Float shift,
  Whole rate) -> Flag;
void jump(Formant &formant, const Tract &tract, Float shift, Whole rate);
auto makeup(const Formant &formant, Whole rate) -> Float;
void settle(Formant &formant, Float vowel, Float shift, Whole rate);
auto tick(Formant &formant, Float in) -> Float;

}  // namespace SOUND::PLUGINS::CORE::FILTER
