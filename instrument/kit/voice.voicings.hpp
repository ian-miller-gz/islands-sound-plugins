// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "kit.hpp"

namespace SOUND::PLUGINS::KIT {

struct Voicing {
  Float tune = 0;
  Float closed = 0;
  Float attack = 0;
  Float drive = 0;
  Float snappy = 0;
  Float spread = 0;
  Float bend = 0;
  Float splash = 0;
};

inline constexpr Voicing VOICINGS[DRUMS] = {
  {.tune = 55, .attack = 0.4f, .drive = 0.2f},
  {.tune = 180, .snappy = 0.6f},
  {.closed = 0.06f},
  {.spread = 0.011f},
  {.tune = 90, .bend = 0.3f},
  {.splash = 0.5f},
  {},
  {.tune = 2500},
  {.tune = 190, .bend = 0.2f}};

}  // namespace SOUND::PLUGINS::KIT
