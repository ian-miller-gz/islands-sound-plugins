// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float CENT = 100;
constexpr Float TRIMMED = 8;
constexpr Float WANDERING = 2;
constexpr CORE::MODULATOR::Seed SALT = 0x2545F491u;

}  // namespace

void SOUND::PLUGINS::DUO::seed(Synth &synth) {
  CORE::VOICE::draw(synth.trims, CORE::MODULATOR::SEED);
  for (Whole part = 0; part < CORE::VOICE::PARTS; ++part)
    CORE::MODULATOR::seed(
      synth.drifts[part],
      CORE::MODULATOR::SEED ^ (CORE::MODULATOR::Seed(part + 1) * ::SALT));
}

void SOUND::PLUGINS::DUO::wander(Synth &synth) {
  for (CORE::MODULATOR::Drift &drift : synth.drifts)
    CORE::MODULATOR::settle(drift, ::WANDERING, synth.rate);
}

auto SOUND::PLUGINS::DUO::stray(Synth &synth, Whole part) -> Float {
  const Float *rows = synth.rows;
  const Float trim = synth.trims[part].pitch * ::TRIMMED * rows[TRIM];
  const Float drift = CORE::MODULATOR::tick(synth.drifts[part]) * rows[DRIFT];
  return (trim + drift) / ::CENT;
}
