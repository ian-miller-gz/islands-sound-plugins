// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "filter.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float ONE = 1.0f;
constexpr Float TEN = 10.0f;
constexpr Float DECIBELS = 20.0f;
constexpr Float PI = 3.14159265f;
constexpr Float MOVED = 0.001f;

auto moved(const FILTER::Formant &formant, Float shift) -> Flag {
  if (formant.breadth != formant.applied) return true;
  for (Whole band = 0; band < FILTER::FORMANTS; ++band) {
    const Float settled = formant.settled[band];
    const Float centre = formant.tract.centres[band] * shift;
    if (std::fabs(centre - settled) > MOVED * settled) return true;
  }
  return false;
}

void apply(FILTER::Formant &formant, Float shift, Whole rate) {
  const FILTER::Tract &tract = formant.tract;
  for (Whole band = 0; band < FILTER::FORMANTS; ++band) {
    const Float centre = tract.centres[band];
    const Float q = centre / (tract.widths[band] * formant.breadth);
    formant.settled[band] = centre * shift;
    FILTER::settle(
      formant.bands[band], FILTER::BAND, formant.settled[band], q, 0, rate);
    formant.gains[band] = std::pow(TEN, tract.levels[band] / DECIBELS);
  }
  formant.applied = formant.breadth;
}

void follow(Float *values, const Float *targets, Float pole) {
  for (Whole band = 0; band < FILTER::FORMANTS; ++band)
    values[band] += pole * (targets[band] - values[band]);
}

}  // namespace

auto SOUND::PLUGINS::CORE::FILTER::glide(
  Formant &formant, const Tract &tract, Float pole, Float shift,
  Whole rate) -> Flag {
  const Float share = bounded(pole);
  ::follow(formant.tract.centres, tract.centres, share);
  ::follow(formant.tract.widths, tract.widths, share);
  ::follow(formant.tract.levels, tract.levels, share);
  if (!::moved(formant, shift)) return false;
  ::apply(formant, shift, rate);
  return true;
}

void SOUND::PLUGINS::CORE::FILTER::jump(
  Formant &formant, const Tract &tract, Float shift, Whole rate) {
  formant.tract = tract;
  ::apply(formant, shift, rate);
}

auto SOUND::PLUGINS::CORE::FILTER::makeup(const Formant &formant, Whole rate)
  -> Float {
  const Float scale = PI * formant.breadth / Float(rate);
  Float sum = 0;
  for (Whole band = 0; band < FORMANTS; ++band) {
    const Float gain = formant.gains[band];
    sum += gain * gain * formant.tract.widths[band] * scale;
  }
  return sum > 0 ? ONE / std::sqrt(sum) : ONE;
}

void SOUND::PLUGINS::CORE::FILTER::settle(
  Formant &formant, Float vowel, Float shift, Whole rate) {
  Tract tract;
  aim(tract, Float(TENOR), vowel, vowel, 0);
  jump(formant, tract, shift, rate);
}

auto SOUND::PLUGINS::CORE::FILTER::tick(Formant &formant, Float in) -> Float {
  Float sum = 0;
  for (Whole band = 0; band < FORMANTS; ++band)
    sum += formant.gains[band] * tick(formant.bands[band], in);
  return sum;
}
