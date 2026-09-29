// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

auto SOUND::ECHO::tick(Track &track, Float in, Float delay, Float drive)
  -> Float {
  const Float out = CORE::LINE::read(track.line, delay);
  const Float damped = CORE::LINE::damp(track.loop, out);
  const Float back = CORE::FILTER::tick(track.cut, damped);
  const Float fed = in + track.loop.feedback * back;
  CORE::LINE::write(track.line, CORE::SHAPER::soft(fed * drive) / drive);
  return out;
}
