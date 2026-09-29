// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float CENT = 100.0f;
constexpr Float FULL = 1.0f;
constexpr Float HALF = 0.5f;

using Change =
  void (*)(VOCODER::Vocoder &, CORE::VOICE::Note &, VOCODER::Carrier &);

void kept(VOCODER::Vocoder &, CORE::VOICE::Note &, VOCODER::Carrier &) {}

void struck(
  VOCODER::Vocoder &vocoder, CORE::VOICE::Note &note,
  VOCODER::Carrier &carrier) {
  const Float pitch = Float(note.pitch) + note.cents / CENT;
  const Float hertz = CORE::PHASE::hertz(pitch);
  CORE::OSCILLATOR::settle(
    carrier.oscillator, hertz, CORE::OSCILLATOR::SQUARE, vocoder.rate);
  const Float start = (CORE::MODULATOR::draw(vocoder.seed) + FULL) * HALF;
  CORE::OSCILLATOR::reset(
    carrier.oscillator, CORE::PHASE::Wheel(start * CORE::PHASE::TURN));
  CORE::ENVELOPE::strike(carrier.gate, vocoder.envelope, note.velocity);
}

void lifted(
  VOCODER::Vocoder &, CORE::VOICE::Note &, VOCODER::Carrier &carrier) {
  CORE::ENVELOPE::lift(carrier.gate);
}

constexpr Change CHANGES[] = {kept, struck, kept, lifted};
static_assert(sizeof(CHANGES) / sizeof(CHANGES[0]) == CORE::VOICE::LIFTED + 1);

using Shape = auto (*)(VOCODER::Carrier &) -> Float;

constexpr Shape SHAPES[] = {
  [](VOCODER::Carrier &carrier) {
    return CORE::OSCILLATOR::tick(carrier.oscillator, CORE::OSCILLATOR::SAW);
  },
  [](VOCODER::Carrier &carrier) {
    return CORE::OSCILLATOR::tick(carrier.oscillator, CORE::OSCILLATOR::PULSE);
  },
  [](VOCODER::Carrier &carrier) { return CORE::NOISE::tick(carrier.white); }};
static_assert(sizeof(SHAPES) / sizeof(SHAPES[0]) == VOCODER::WAVES);

void voice(VOCODER::Vocoder &vocoder, CORE::VOICE::Note &note) {
  const Whole at = Whole(&note - vocoder.allocator.notes);
  if (at >= VOCODER::CARRIERS || note.change > CORE::VOICE::LIFTED) return;
  CHANGES[note.change](vocoder, note, vocoder.carriers[at]);
}

auto sounded(
  VOCODER::Vocoder &vocoder, CORE::VOICE::Note &note, VOCODER::Carrier &carrier,
  Shape shape) -> Float {
  const Float level = CORE::ENVELOPE::tick(carrier.gate, vocoder.envelope);
  note.level = level;
  if (!CORE::ENVELOPE::sounding(carrier.gate)) note.sounding = false;
  return shape(carrier) * level;
}

}  // namespace

void SOUND::VOCODER::apply(
  Vocoder &vocoder, const AUDIO::PLUGIN::Event &event) {
  const Whole row = CORE::VOICE::apply(
    vocoder.allocator, event,
    [&vocoder](CORE::VOICE::Note &note) { ::voice(vocoder, note); });
  if (row >= PARAMETERS) return;
  vocoder.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(vocoder);
}

auto SOUND::VOCODER::play(Vocoder &vocoder) -> Float {
  const Shape shape = ::SHAPES[vocoder.wave < WAVES ? vocoder.wave : SAW];
  Float sum = 0;
  for (Whole at = 0; at < CARRIERS; ++at) {
    CORE::VOICE::Note &note = vocoder.allocator.notes[at];
    if (note.sounding)
      sum += ::sounded(vocoder, note, vocoder.carriers[at], shape);
  }
  return sum * vocoder.level;
}
