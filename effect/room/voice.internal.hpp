// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "room.hpp"
#include "../../core/block/block.hpp"
#include "../../core/reverb/reverb.hpp"

namespace SOUND::ROOM {

constexpr Whole MONO = 1;
constexpr Float DAMP = 8000.0f;
constexpr Float EARLY = 0.5f;
constexpr Float HALF = 0.5f;
constexpr Float UNITY = 1.0f;

struct Effect {
  Whole rate = 0;
  Whole channels = 0;
  CORE::REVERB::Reflections reflections;
  CORE::REVERB::Network network;
  Float rows[PARAMETERS] = {};
  Float dry = UNITY;
  Float wet = 0;
  CORE::BLOCK::Meter meter;
};

void build(Effect &effect);
void settle(Effect &effect);
void apply(Effect &effect, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::ROOM
