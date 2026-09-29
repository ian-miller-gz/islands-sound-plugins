// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float LO = -72;
constexpr Float OCTAVE = 12;
constexpr Float CENT = 100;
constexpr Float SNAP = 0.005f;
constexpr Float BLOCKED = 10;
constexpr Float PORTION = 1.0f / 3.0f;

constexpr Float OFFSETS[] = {LO, -2 * OCTAVE, -OCTAVE, 0, OCTAVE, 2 * OCTAVE};
static_assert(std::size(OFFSETS) == LADDER::FEET + 1);

constexpr Float TRACKS[] = {0, PORTION, 2 * PORTION, 1};
static_assert(std::size(TRACKS) == LADDER::TRACKS + 1);

struct Knobs {
  Whole range;
  Whole wave;
  Whole frequency;
  Whole level;
};

constexpr Knobs KNOBS[LADDER::OSCILLATORS] = {
  {LADDER::RANGE1, LADDER::WAVE1, LADDER::PARAMETERS, LADDER::LEVEL1},
  {LADDER::RANGE2, LADDER::WAVE2, LADDER::FREQUENCY2, LADDER::LEVEL2},
  {LADDER::RANGE3, LADDER::WAVE3, LADDER::FREQUENCY3, LADDER::LEVEL3}};

constexpr Whole PICKS[LADDER::OSCILLATORS][LADDER::POSITIONS] = {
  {LADDER::TRIANGLE, LADDER::SHARKTOOTH, LADDER::SAW, LADDER::SQUARE,
   LADDER::WIDE, LADDER::NARROW},
  {LADDER::TRIANGLE, LADDER::SHARKTOOTH, LADDER::SAW, LADDER::SQUARE,
   LADDER::WIDE, LADDER::NARROW},
  {LADDER::TRIANGLE, LADDER::SHARKTOOTH, LADDER::REVERSE, LADDER::SAW,
   LADDER::SQUARE, LADDER::WIDE}};

auto held(const LADDER::Synth &synth, Whole index) -> Float {
  return index < LADDER::PARAMETERS ? synth.rows[index] : 0;
}

void source(LADDER::Synth &synth, Whole at) {
  const Knobs &knobs = KNOBS[at];
  LADDER::Source &source = synth.sources[at];
  source.shape = PICKS[at][Whole(synth.rows[knobs.wave])];
  source.offset =
    OFFSETS[Whole(synth.rows[knobs.range])] + ::held(synth, knobs.frequency);
  source.level = synth.rows[knobs.level];
}

void contour(
  LADDER::Contour &contour, const CORE::ENVELOPE::Envelope &envelope,
  Whole rate) {
  contour.envelope = envelope;
  contour.envelope.curve = CORE::ENVELOPE::EXPONENTIAL;
  contour.envelope.trigger = CORE::ENVELOPE::RESUME;
  CORE::ENVELOPE::shape(contour.envelope, rate);
}

}  // namespace

void SOUND::LADDER::settle(Synth &synth) {
  const Float *rows = synth.rows;
  for (Whole at = 0; at < OSCILLATORS; ++at) ::source(synth, at);
  const Flag held = rows[RELEASE] > 0;
  const Float close = held ? rows[CLOSE] : SNAP;
  const Float fade = held ? rows[DECAY] : SNAP;
  ::contour(
    synth.filter,
    CORE::ENVELOPE::ADSR::create(rows[SWEEP], rows[CLOSE], rows[FLOOR], close),
    synth.rate);
  ::contour(
    synth.loudness,
    CORE::ENVELOPE::ADSR::create(
      rows[ATTACK], rows[DECAY], rows[SUSTAIN], fade),
    synth.rate);
  synth.allocator.glide.time = rows[GLIDE];
  CORE::VOICE::settle(synth.allocator, synth.rate);
  synth.tune = rows[TUNE] / CENT;
  synth.tracking = ::TRACKS[Whole(rows[TRACKING])];
  CORE::SHAPER::settle(synth.blocker, BLOCKED, synth.rate);
}
