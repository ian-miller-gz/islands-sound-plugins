// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float CENT = 100;
constexpr Float TRIMMED = 8;
constexpr Float OPENED = 0.25f;
constexpr Float STRETCHED = 0.1f;
constexpr Float UNITY = 1;
constexpr Float WANDERING = 2;
constexpr CORE::MODULATOR::Seed SCATTER = 0x51A7C3E5u;

}  // namespace

void SOUND::PLUGINS::MULTIMODE::seed(Synth &synth) {
  CORE::VOICE::draw(synth.trims, CORE::MODULATOR::SEED);
  for (Whole at = 0; at < VOICES; ++at)
    CORE::MODULATOR::seed(
      synth.voices[at].drift,
      CORE::MODULATOR::SEED ^ (CORE::MODULATOR::Seed(at + 1) * ::SCATTER));
}

void SOUND::PLUGINS::MULTIMODE::wander(Synth &synth) {
  for (Voice &voice : synth.voices)
    CORE::MODULATOR::settle(voice.drift, ::WANDERING, synth.rate / STRIDE);
}

void SOUND::PLUGINS::MULTIMODE::drift(Synth &synth, Voice &voice) {
  voice.wander = CORE::MODULATOR::tick(voice.drift) * synth.rows[DRIFT];
}

auto SOUND::PLUGINS::MULTIMODE::stray(const Synth &synth, Whole at) -> Float {
  const Float trim = synth.trims[at].pitch * ::TRIMMED * synth.rows[TRIM];
  return (trim + synth.voices[at].wander) / ::CENT;
}

auto SOUND::PLUGINS::MULTIMODE::open(const Synth &synth, Whole at) -> Float {
  return synth.trims[at].cutoff * ::OPENED * synth.rows[TRIM];
}

auto SOUND::PLUGINS::MULTIMODE::stretch(const Synth &synth, Whole at) -> Float {
  return ::UNITY + synth.trims[at].time * ::STRETCHED * synth.rows[TRIM];
}
