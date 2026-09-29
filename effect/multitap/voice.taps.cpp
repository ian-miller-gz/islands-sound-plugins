// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

auto SOUND::PLUGINS::MULTITAP::tick(Effect &effect, Float in) -> Stereo {
  Stereo echo;
  Float back = 0;
  for (Whole at = 0; at < TAPS; ++at) {
    Tap &tap = effect.taps[at];
    const Float delay = CORE::MODULATOR::tick(tap.time, tap.frames);
    const Float out = CORE::LINE::read(effect.line, delay);
    echo.left += tap.left * out;
    echo.right += tap.right * out;
    if (at == effect.last) back = out;
  }
  CORE::LINE::write(effect.line, in + effect.feedback * back);
  return echo;
}
