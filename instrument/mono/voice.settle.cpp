// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float OCTAVE = 12;
constexpr Float CENT = 100;
constexpr Float BLOCKED = 10;
constexpr Float HALVED = 2;
constexpr Float QUARTERED = 4;
constexpr Float QUARTER = 0.25f;
constexpr Float SQUARE = CORE::OSCILLATOR::SQUARE;

constexpr Float OFFSETS[] = {-OCTAVE, 0, OCTAVE, 2 * OCTAVE};
static_assert(std::size(OFFSETS) == std::size(MONO::RANGES));

struct Division {
  Float divisor;
  Float width;
};

constexpr Division DIVISIONS[] = {
  {HALVED, SQUARE}, {QUARTERED, SQUARE}, {QUARTERED, QUARTER}};
static_assert(std::size(DIVISIONS) == std::size(MONO::DOWNS));

struct Portamento {
  Flag moving;
  Whole slide;
};

constexpr Portamento PORTAMENTI[] = {
  {false, CORE::VOICE::ALWAYS},
  {true, CORE::VOICE::LEGATO},
  {true, CORE::VOICE::ALWAYS}};
static_assert(std::size(PORTAMENTI) == std::size(MONO::SLIDES));

constexpr Whole LFOS[] = {
  CORE::MODULATOR::TRIANGLE, CORE::MODULATOR::SQUARE, CORE::MODULATOR::SAMPLE,
  CORE::MODULATOR::SAMPLE};
static_assert(std::size(LFOS) == std::size(MONO::WAVES));

void contour(
  MONO::Contour &contour, const CORE::ENVELOPE::Envelope &envelope,
  Whole rate) {
  contour.envelope = envelope;
  contour.envelope.curve = CORE::ENVELOPE::EXPONENTIAL;
  contour.envelope.trigger = CORE::ENVELOPE::RESUME;
  CORE::ENVELOPE::shape(contour.envelope, rate);
}

void glide(MONO::Synth &synth) {
  const Portamento &portamento = PORTAMENTI[Whole(synth.rows[MONO::SLIDE])];
  CORE::VOICE::Glide &glide = synth.allocator.glide;
  glide.time = portamento.moving ? synth.rows[MONO::PORTAMENTO] : 0;
  glide.slide = portamento.slide;
  CORE::VOICE::settle(synth.allocator, synth.rate);
}

void swing(MONO::Synth &synth) {
  synth.lfo.wave = LFOS[Whole(synth.rows[MONO::WAVE])];
  synth.lfo.hertz = synth.rows[MONO::RATE];
  CORE::MODULATOR::settle(synth.lfo, synth.rate);
}

}  // namespace

void SOUND::MONO::settle(Synth &synth) {
  const Float *rows = synth.rows;
  const Division &division = DIVISIONS[Whole(rows[DOWN])];
  synth.divisor = division.divisor;
  synth.narrow = division.width;
  synth.offset = OFFSETS[Whole(rows[RANGE])] + rows[TUNE] / CENT;
  ::contour(
    synth.envelope,
    CORE::ENVELOPE::ADSR::create(
      rows[ATTACK], rows[DECAY], rows[SUSTAIN], rows[RELEASE]),
    synth.rate);
  ::contour(synth.gate, CORE::ENVELOPE::GATE::create(0), synth.rate);
  synth.allocator.legato = Whole(rows[TRIGGER]) != TRIGGERED;
  ::glide(synth);
  ::swing(synth);
  CORE::SHAPER::settle(synth.blocker, BLOCKED, synth.rate);
}
