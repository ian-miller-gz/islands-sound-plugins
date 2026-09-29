// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float FULL = 1.0f;
constexpr Float CENT = 100.0f;

void bands(VOCODER::Vocoder &vocoder) {
  const Float *rows = vocoder.rows;
  const Float span = rows[VOCODER::HIGH] / rows[VOCODER::LOW];
  const Float step = std::pow(span, FULL / Float(VOCODER::BANDS - 1));
  const Float root = std::sqrt(step);
  const Float q = FULL / (root - FULL / root);
  const Float shift = CORE::PHASE::ratio(rows[VOCODER::SHIFT] * CENT);
  Float centre = rows[VOCODER::LOW];
  for (VOCODER::Band &band : vocoder.bands) {
    CORE::FILTER::settle(
      band.analysis, CORE::FILTER::BAND, centre, q, 0, vocoder.rate);
    CORE::FILTER::settle(
      band.synthesis, CORE::FILTER::BAND, centre * shift, q, 0, vocoder.rate);
    band.follower.attack = rows[VOCODER::ATTACK];
    band.follower.release = rows[VOCODER::RELEASE];
    CORE::DYNAMICS::settle(band.follower, vocoder.rate);
    centre *= step;
  }
}

void carrier(VOCODER::Vocoder &vocoder) {
  const Float *rows = vocoder.rows;
  const Whole unison = Whole(rows[VOCODER::UNISON]);
  vocoder.allocator.unison = unison;
  vocoder.allocator.detune = rows[VOCODER::DETUNE];
  vocoder.level = FULL / std::sqrt(Float(unison < 1 ? 1 : unison));
  vocoder.wave = Whole(rows[VOCODER::WAVE]);
}

}  // namespace

void SOUND::PLUGINS::VOCODER::settle(Vocoder &vocoder) {
  vocoder.gain = vocoder.rows[GAIN] * LOUDNESS;
  ::bands(vocoder);
  ::carrier(vocoder);
}
