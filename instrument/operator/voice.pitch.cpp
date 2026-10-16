// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float SPANS[] = {OPERATOR::CENTRE, OPERATOR::TOP - OPERATOR::CENTRE};

}  // namespace

void SOUND::PLUGINS::OPERATOR::trace(Synth &synth) {
  CORE::ENVELOPE::Segments &contour = synth.contour;
  for (Whole stage = 0; stage < STAGES; ++stage) {
    contour.rates[stage] = synth.rows[PACE + stage];
    contour.levels[stage] = synth.rows[HEIGHT + stage];
  }
  contour.curve = CORE::ENVELOPE::LINEAR;
  contour.depth = 0;
  CORE::ENVELOPE::shape(contour, synth.rate);
}

auto SOUND::PLUGINS::OPERATOR::sweep(const Synth &synth, Voice &voice)
  -> Float {
  const Float height = CORE::ENVELOPE::tick(voice.contour, synth.contour) * TOP;
  const Float offset = height - CENTRE;
  return synth.rows[SWEEP] * offset / ::SPANS[offset > 0];
}
