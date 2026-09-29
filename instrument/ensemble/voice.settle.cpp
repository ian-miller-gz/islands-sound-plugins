// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float CENT = 100;
constexpr Float KEYING = 0.005f;

auto swell(const Float *rows, Whole rate) -> CORE::ENVELOPE::Envelope {
  auto envelope =
    CORE::ENVELOPE::AR::create(rows[ENSEMBLE::ATTACK], rows[ENSEMBLE::RELEASE]);
  envelope.curve = CORE::ENVELOPE::EXPONENTIAL;
  envelope.trigger = CORE::ENVELOPE::RESUME;
  CORE::ENVELOPE::shape(envelope, rate);
  return envelope;
}

void tune(ENSEMBLE::Synth &synth) {
  const Float *rows = synth.rows;
  const Float offset = rows[ENSEMBLE::TUNE] / CENT + rows[ENSEMBLE::BEND];
  for (Whole at = 0; at < ENSEMBLE::CLASSES; ++at) {
    const Float pitch = Float(ENSEMBLE::TOP + at) + offset;
    const Float turns = CORE::PHASE::hertz(pitch) / Float(synth.rate);
    synth.steps[at] = ENSEMBLE::Counter(turns * CORE::PHASE::TURN);
  }
}

void voice(ENSEMBLE::Synth &synth) {
  for (Whole at = 0; at < ENSEMBLE::REGISTERS; ++at)
    CORE::FILTER::settle(
      synth.tones[at], CORE::FILTER::LOW, ENSEMBLE::PANEL[at].cutoff,
      CORE::FILTER::FLAT, 0, synth.rate);
}

}  // namespace

void SOUND::PLUGINS::ENSEMBLE::settle(Synth &synth) {
  synth.envelope = ::swell(synth.rows, synth.rate);
  synth.ramp = CORE::MODULATOR::delta(KEYING, synth.rate);
  ::tune(synth);
  ::voice(synth);
  settle(synth.ensemble, synth.rows, synth.rate);
}
