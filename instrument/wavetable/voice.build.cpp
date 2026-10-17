// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr WAVETABLE::Spectrum SPECTRA[] = {
  WAVETABLE::SPECTRUM::sine,    WAVETABLE::SPECTRUM::harmonic,
  WAVETABLE::SPECTRUM::saw,     WAVETABLE::SPECTRUM::square,
  WAVETABLE::SPECTRUM::hollow,  WAVETABLE::SPECTRUM::triangle,
  WAVETABLE::SPECTRUM::pulse,   WAVETABLE::SPECTRUM::organ,
  WAVETABLE::SPECTRUM::octaves, WAVETABLE::SPECTRUM::formant,
  WAVETABLE::SPECTRUM::vowel,   WAVETABLE::SPECTRUM::comb,
  WAVETABLE::SPECTRUM::sync,    WAVETABLE::SPECTRUM::prime,
  WAVETABLE::SPECTRUM::scatter, WAVETABLE::SPECTRUM::resonant};
static_assert(std::size(SPECTRA) == WAVETABLE::TABLES);

void key(
  CORE::OSCILLATOR::Table &table, WAVETABLE::Spectrum spectrum, Whole frame) {
  Float partials[WAVETABLE::PARTIALS];
  const Float place = Float(frame) / WAVETABLE::LAST;
  for (Whole at = 0; at < WAVETABLE::PARTIALS; ++at)
    partials[at] = spectrum(place, Float(at + 1));
  CORE::OSCILLATOR::draw(table, frame, partials, WAVETABLE::PARTIALS);
}

}  // namespace

void SOUND::PLUGINS::WAVETABLE::build(
  CORE::OSCILLATOR::Table &table, Whole layout) {
  const Spectrum spectrum = ::SPECTRA[layout < TABLES ? layout : 0];
  CORE::OSCILLATOR::build(table, FRAMES, CORE::OSCILLATOR::LEVELS);
  for (Whole at = 0; at < KEYS; ++at) ::key(table, spectrum, at * SPAN);
  for (Whole at = 0; at + 1 < KEYS; ++at)
    CORE::OSCILLATOR::blend(table, at * SPAN, (at + 1) * SPAN);
}
