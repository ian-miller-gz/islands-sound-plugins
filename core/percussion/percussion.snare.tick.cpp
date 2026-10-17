// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussion.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float TONES = 0.7f;
constexpr Float MODAL = 0.5f;
constexpr Float ONE = 1.0f;

auto toned(PERCUSSION::Snare &snare) -> Float {
  const Float heads =
    (PERCUSSION::tick(snare.low) + PERCUSSION::tick(snare.high)) * TONES;
  Float modes = 0;
  for (PERCUSSION::Sweep &mode : snare.modes) modes += PERCUSSION::tick(mode);
  const Float body = snare.setting.body;
  const Float strokes =
    PERCUSSION::tick(snare.ping) + PERCUSSION::tick(snare.click);
  return heads * (ONE - body) + modes * MODAL * body + strokes;
}

auto rattled(PERCUSSION::Snare &snare) -> Float {
  if (snare.holding == 0) return ENVELOPE::tick(snare.gate, snare.rattle);
  --snare.holding;
  return ENVELOPE::FULL * snare.gate.scale;
}

}  // namespace

auto SOUND::PLUGINS::CORE::PERCUSSION::tick(Snare &snare) -> Float {
  if (!sounding(snare)) return 0;
  const Float tones = ::toned(snare);
  const Float banded = FILTER::tick(snare.band, NOISE::tick(snare.white));
  const Float noise = FILTER::tick(snare.floor, banded);
  const Float swell = ::rattled(snare);
  const Float rattle = noise * swell * snare.setting.snappy * snare.level;
  return SHAPER::soft(tones + rattle);
}
