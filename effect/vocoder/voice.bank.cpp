// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float THRESHOLD = 0.5f;
constexpr Float FLOOR = 0.001f;
constexpr Float VOICED = 0;
constexpr Float UNVOICED = 1.0f;

auto hissed(VOCODER::Hiss &hiss, Float in) -> Float {
  const Float high = CORE::FILTER::tick(hiss.high, in);
  const Float air = CORE::DYNAMICS::tick(hiss.air, high);
  const Float whole = CORE::DYNAMICS::tick(hiss.whole, in);
  const Flag unvoiced = whole > FLOOR && air > whole * THRESHOLD;
  return CORE::MODULATOR::tick(hiss.weight, unvoiced ? UNVOICED : VOICED);
}

}  // namespace

auto SOUND::VOCODER::vocode(Vocoder &vocoder, Float modulator, Float carrier)
  -> Float {
  const Float weight =
    ::hissed(vocoder.hiss, modulator) * vocoder.rows[UNVOICED];
  const Float noise = CORE::NOISE::tick(vocoder.hiss.white);
  const Float source = carrier + (noise - carrier) * weight;
  Float sum = 0;
  for (Band &band : vocoder.bands) {
    const Float heard = CORE::FILTER::tick(band.analysis, modulator);
    const Float level = CORE::DYNAMICS::tick(band.follower, heard);
    sum += CORE::FILTER::tick(band.synthesis, source) * level;
  }
  return sum;
}
