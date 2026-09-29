// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float LEVEL = 0.25f;

}  // namespace

auto SOUND::PLUGINS::ENSEMBLE::sound(Synth &synth) -> Pair {
  const Float swell = CORE::ENVELOPE::tick(synth.gate, synth.envelope);
  const Sums sums = divide(synth);
  Float mixed = 0;
  for (Whole at = 0; at < REGISTERS; ++at) {
    const Register &stop = PANEL[at];
    const Float fed = stop.eight * sums.eight + stop.sixteen * sums.sixteen;
    mixed += synth.rows[VIOLIN + at] * CORE::FILTER::tick(synth.tones[at], fed);
  }
  return tick(synth.ensemble, LEVEL * swell * mixed);
}
