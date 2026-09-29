// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float UNITY = 1.0f;
constexpr Float HALF = 0.5f;
constexpr Float MILLI = 1000.0f;

void place(HARMONIZER::Part &part, Float level, Float pan) {
  part.left = level * (pan > 0 ? ::UNITY - pan : ::UNITY);
  part.right = level * (pan < 0 ? ::UNITY + pan : ::UNITY);
}

void struck(HARMONIZER::Harmonizer &harmonizer, CORE::VOICE::Note &note) {
  const Whole at = Whole(&note - harmonizer.allocator.notes);
  if (at >= HARMONIZER::VOICES || note.change != CORE::VOICE::STRUCK) return;
  HARMONIZER::Part &part = harmonizer.parts[at];
  if (part.gate.value < HARMONIZER::QUIET) part.fresh = true;
}

}  // namespace

void SOUND::PLUGINS::HARMONIZER::build(Harmonizer &harmonizer) {
  CORE::PITCH::build(
    harmonizer.detector, harmonizer.rate, LOWEST, TOPMOST, HOP);
  harmonizer.allocator.count = VOICES;
  for (Part &part : harmonizer.parts) {
    CORE::PITCH::build(part.grains, WINDOW, harmonizer.rate);
    CORE::PITCH::build(part.formant, WINDOW, harmonizer.rate);
    part.gate.time = FADE;
    CORE::MODULATOR::settle(part.gate, harmonizer.rate);
  }
}

void SOUND::PLUGINS::HARMONIZER::settle(Harmonizer &harmonizer) {
  const Float *rows = harmonizer.rows;
  harmonizer.keep = rows[FORMANT] >= ::HALF;
  for (Whole at = 0; at < VOICES; ++at) {
    Part &part = harmonizer.parts[at];
    ::place(part, rows[LEVEL + at], rows[PAN + at]);
    part.glide.time = rows[RETUNE] / ::MILLI;
    CORE::MODULATOR::settle(part.glide, harmonizer.rate / STRIDE);
  }
}

void SOUND::PLUGINS::HARMONIZER::apply(
  Harmonizer &harmonizer, const AUDIO::PLUGIN::Event &event) {
  const Whole row = CORE::VOICE::apply(
    harmonizer.allocator, event,
    [&harmonizer](CORE::VOICE::Note &note) { ::struck(harmonizer, note); });
  if (row >= PARAMETERS) return;
  harmonizer.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(harmonizer);
}
