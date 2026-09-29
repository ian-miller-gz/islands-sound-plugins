// SPDX-License-Identifier: AGPL-3.0-or-later
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Float CENT = 100;
constexpr Float HALF = 0.5f;
constexpr Float QUARTER = 240;
constexpr Whole LONG = 1;
constexpr Whole SHORT = 6;

constexpr Float PERIODS[] = {4,   8,   16,  32,  64,  96,   128,  160,
                             202, 254, 380, 508, 762, 1016, 2034, 4068};

struct Rows {
  Whole level;
  Whole decay;
  Whole sustain;
};

constexpr Rows SHAPES[CHIP::ENVELOPES] = {
  {CHIP::LEVEL1, CHIP::DECAY1, CHIP::SUSTAIN1},
  {CHIP::LEVEL2, CHIP::DECAY2, CHIP::SUSTAIN2},
  {CHIP::LEVEL4, CHIP::DECAY4, CHIP::SUSTAIN4}};

constexpr Whole DUTIES[CHIP::PULSES] = {CHIP::DUTY1, CHIP::DUTY2};

struct Corner {
  Whole kind;
  Float cutoff;
};

constexpr Corner CORNERS[CHIP::FILTERS] = {
  {CORE::FILTER::HIGH, 90},
  {CORE::FILTER::HIGH, 440},
  {CORE::FILTER::LOW, 14000}};

void hiss(CHIP::Synth &synth) {
  const Whole period = Whole(synth.rows[CHIP::PERIOD]);
  const Float cycles = PERIODS[period < std::size(PERIODS) ? period : 0];
  synth.noise.tap = synth.rows[CHIP::MODE] > HALF ? SHORT : LONG;
  synth.noise.clocks = CHIP::CPU / cycles / Float(synth.rate);
}

}  // namespace

void SOUND::CHIP::settle(Synth &synth) {
  const Float *rows = synth.rows;
  for (Whole at = 0; at < ENVELOPES; ++at)
    synth.shapes[at] = {
      Whole(rows[::SHAPES[at].level]), Whole(rows[::SHAPES[at].decay]),
      Whole(rows[::SHAPES[at].sustain])};
  for (Whole at = 0; at < PULSES; ++at)
    synth.pulses[at].duty = Whole(rows[::DUTIES[at]]);
  ::hiss(synth);
  synth.frame.step = CORE::PHASE::step(::QUARTER, synth.rate);
  synth.arpeggio.step = CORE::PHASE::step(rows[SPEED], synth.rate);
  for (Whole at = 0; at < FILTERS; ++at)
    CORE::FILTER::settle(
      synth.filters[at], ::CORNERS[at].kind, ::CORNERS[at].cutoff, synth.rate);
  synth.tune = rows[TUNE] / ::CENT;
  synth.stale = true;
}
