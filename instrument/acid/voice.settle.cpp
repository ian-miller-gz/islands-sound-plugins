// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float ATTACK = 0.003f;
constexpr Float HOLD = 3;
constexpr Float RELEASE = 0.008f;
constexpr Float COUPLING = 44;
constexpr Float BLOCKED = 10;
constexpr Float SWIFT = 0.01f;
constexpr Float SWEEP = 0.09f;

constexpr Whole SHAPES[] = {CORE::OSCILLATOR::SAW, CORE::OSCILLATOR::PULSE};
static_assert(std::size(SHAPES) == std::size(ACID::WAVES));

auto shaped(CORE::ENVELOPE::Envelope envelope, Whole rate)
  -> CORE::ENVELOPE::Envelope {
  envelope.curve = CORE::ENVELOPE::EXPONENTIAL;
  envelope.trigger = CORE::ENVELOPE::RESUME;
  CORE::ENVELOPE::shape(envelope, rate);
  return envelope;
}

void contour(ACID::Synth &synth) {
  const Float decays[ACID::ACCENTS] = {synth.rows[ACID::DECAY], ACID::SHORTEST};
  for (Whole accent = 0; accent < ACID::ACCENTS; ++accent)
    synth.contours[accent] =
      ::shaped(CORE::ENVELOPE::AD::create(ATTACK, decays[accent]), synth.rate);
  synth.amplifier = ::shaped(
    CORE::ENVELOPE::ADSR::create(ATTACK, HOLD, 0, RELEASE), synth.rate);
}

void glide(ACID::Synth &synth) {
  synth.allocator.glide.time = synth.rows[ACID::SLIDE];
  synth.allocator.glide.slide = CORE::VOICE::LEGATO;
  CORE::VOICE::settle(synth.allocator, synth.rate);
}

}  // namespace

void SOUND::ACID::settle(Synth &synth) {
  const Float *rows = synth.rows;
  ::contour(synth);
  ::glide(synth);
  synth.push.time = SWIFT + SWEEP * rows[RESONANCE];
  CORE::MODULATOR::settle(synth.push, synth.rate);
  synth.wave = SHAPES[Whole(rows[WAVE])];
  CORE::FILTER::settle(
    synth.coupling, CORE::FILTER::HIGH, COUPLING, synth.rate);
  CORE::SHAPER::settle(synth.blocker, BLOCKED, synth.rate);
}
