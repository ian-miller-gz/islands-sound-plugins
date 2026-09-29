// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float UNITY = 1.0f;

auto capped(Float gain) -> Float { return gain > UNITY ? UNITY : gain; }

}  // namespace

void SOUND::PLUGINS::MIXER::settle(Module &module) {
  const Float *rows = module.rows;
  const Flag panned = module.channels == STEREO;
  for (Whole in = 0; in < INS; ++in) {
    const Float level = rows[LEVEL + in] * rows[GAIN];
    const Float pan = panned ? rows[PAN + in] : 0;
    module.weights[in][0] = level * ::capped(UNITY - pan);
    module.weights[in][1] = level * ::capped(UNITY + pan);
  }
}

void SOUND::PLUGINS::MIXER::apply(
  Module &module, const AUDIO::PLUGIN::Event &event) {
  if (event.kind != AUDIO::PLUGIN::Event::CONTROLLER) return;
  if (event.index >= PARAMETERS) return;
  module.rows[event.index] =
    CORE::TABLE::clamped(SHEET, event.index, event.value);
  settle(module);
}
