// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float OCTAVE = 12;
constexpr Float CENT = 100;
constexpr Float SNAP = 0.005f;
constexpr Float BLOCKED = 10;

constexpr Float OFFSETS[] = {-2 * OCTAVE, -OCTAVE, 0};
static_assert(std::size(OFFSETS) == std::size(BULL::FEET));

constexpr Whole SHAPES[] = {CORE::OSCILLATOR::SAW, CORE::OSCILLATOR::PULSE};
static_assert(std::size(SHAPES) == std::size(BULL::WAVES));

void contour(
  BULL::Contour &contour, const CORE::ENVELOPE::Envelope &envelope,
  Whole rate) {
  contour.envelope = envelope;
  contour.envelope.curve = CORE::ENVELOPE::EXPONENTIAL;
  contour.envelope.trigger = CORE::ENVELOPE::RESUME;
  CORE::ENVELOPE::shape(contour.envelope, rate);
}

auto loudness(const Float *chain) -> CORE::ENVELOPE::Envelope {
  if (chain[BULL::SUSTAIN] > 0)
    return CORE::ENVELOPE::AR::create(chain[BULL::ATTACK], chain[BULL::DECAY]);
  return CORE::ENVELOPE::ADSR::create(
    chain[BULL::ATTACK], chain[BULL::DECAY], 0, SNAP);
}

}  // namespace

void SOUND::PLUGINS::BULL::settle(Synth &synth) {
  compose(synth);
  const Float *chain = synth.chain;
  synth.waves[FIRST] = ::SHAPES[Whole(chain[WAVEA])];
  synth.waves[SECOND] = ::SHAPES[Whole(chain[WAVEB])];
  synth.offset = ::OFFSETS[Whole(chain[OCTAVE])] + chain[TUNE] / ::CENT;
  synth.beat = CORE::PHASE::ratio(chain[BEAT]);
  ::contour(
    synth.filter, CORE::ENVELOPE::AD::create(chain[SWEEP], chain[CLOSE]),
    synth.rate);
  ::contour(synth.loudness, ::loudness(chain), synth.rate);
  synth.allocator.glide.time = chain[GLIDE];
  CORE::VOICE::settle(synth.allocator, synth.rate);
  CORE::SHAPER::settle(synth.blocker, ::BLOCKED, synth.rate);
}
