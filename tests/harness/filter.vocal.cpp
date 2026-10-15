// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>
#include <cstdio>

#include "../../core/filter/filter.hpp"
#include "../../core/noise/noise.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole RATE = 48000;
constexpr Whole SECONDS = 2;
constexpr Whole CALLS = 40;
constexpr Float TENOR = Float(CORE::FILTER::TENOR);
constexpr Float BETWEEN = 1.5f;
constexpr Float SUNG = 300;
constexpr Float POLE = 0.5f;
constexpr Float FULL = 1;

void show(STRING::Hot word, const Float *values) {
  std::printf("%s", word);
  for (Whole at = 0; at < CORE::FILTER::FORMANTS; ++at)
    std::printf(" %.9g", double(values[at]));
  std::printf("\n");
}

auto aimed(Float voice, Whole vowel) -> CORE::FILTER::Tract {
  CORE::FILTER::Tract tract;
  CORE::FILTER::aim(tract, voice, Float(vowel), Float(vowel), 0);
  return tract;
}

void shape() {
  const CORE::FILTER::Tract open = ::aimed(TENOR, CORE::FILTER::A);
  ::show("centres", open.centres);
  ::show("widths", open.widths);
  ::show("levels", open.levels);
  CORE::FILTER::Tract close = ::aimed(TENOR, CORE::FILTER::I);
  ::show("close", close.centres);
  ::show("between", ::aimed(BETWEEN, CORE::FILTER::A).centres);
  CORE::FILTER::bend(close, SUNG, 0);
  ::show("bent", close.centres);
  ::show("plain", close.levels);
  CORE::FILTER::bend(close, 0, FULL);
  ::show("kept", close.centres);
  ::show("effort", close.levels);
}

void travel() {
  CORE::FILTER::Formant formant;
  CORE::FILTER::jump(formant, ::aimed(TENOR, CORE::FILTER::A), FULL, RATE);
  const CORE::FILTER::Tract close = ::aimed(TENOR, CORE::FILTER::I);
  Flag moved = CORE::FILTER::glide(formant, close, POLE, FULL, RATE);
  const Float first = formant.tract.centres[0];
  std::printf("glide %.9g %d\n", double(first), int(moved));
  for (Whole call = 1; call < CALLS; ++call)
    moved = CORE::FILTER::glide(formant, close, POLE, FULL, RATE);
  std::printf("glided %d\n", int(moved));
}

void balance() {
  CORE::FILTER::Formant formant;
  CORE::FILTER::jump(formant, ::aimed(TENOR, CORE::FILTER::A), FULL, RATE);
  const Float gain = CORE::FILTER::makeup(formant, RATE);
  CORE::NOISE::White white;
  Float in = 0, out = 0;
  for (Whole at = 0; at < SECONDS * RATE; ++at) {
    const Float sample = CORE::NOISE::tick(white);
    const Float made = gain * CORE::FILTER::tick(formant, sample);
    in += sample * sample;
    out += made * made;
  }
  std::printf("makeup %.9g\n", double(std::sqrt(out / in)));
}

void stand() {
  CORE::FILTER::Formant formant;
  CORE::FILTER::settle(formant, Float(CORE::FILTER::A), FULL, RATE);
  ::show("settled", formant.tract.centres);
}

}  // namespace

namespace SOUND::PLUGINS::HARNESS::FILTER {

void vocal() {
  ::shape();
  ::travel();
  ::balance();
  ::stand();
}

}  // namespace SOUND::PLUGINS::HARNESS::FILTER
