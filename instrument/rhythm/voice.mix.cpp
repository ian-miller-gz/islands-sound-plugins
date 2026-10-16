// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;
using CORE::PERCUSSION::tick;

using Tick = auto (*)(RHYTHM::Box &box) -> Float;

constexpr Tick TICKS[RHYTHM::VOICES] = {
  [](RHYTHM::Box &box) -> Float { return tick(box.kick); },
  [](RHYTHM::Box &box) -> Float { return tick(box.snare); },
  [](RHYTHM::Box &box) -> Float { return tick(box.hat); },
  [](RHYTHM::Box &box) -> Float { return tick(box.cymbal); },
  [](RHYTHM::Box &box) -> Float { return tick(box.tom); },
  [](RHYTHM::Box &box) -> Float { return tick(box.clap); },
  [](RHYTHM::Box &box) -> Float { return tick(box.rim); },
  [](RHYTHM::Box &box) -> Float { return tick(box.cowbell); }};

}  // namespace

auto SOUND::PLUGINS::RHYTHM::mix(Box &box) -> Float {
  Float sum = 0;
  for (Whole voice = 0; voice < VOICES; ++voice)
    sum += ::TICKS[voice](box) * box.rows[LEVELLED[voice]];
  return CORE::SHAPER::soft(sum);
}
