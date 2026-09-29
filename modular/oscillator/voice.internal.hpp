// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "oscillator.hpp"
#include "../../core/block/block.hpp"
#include "../../core/oscillator/oscillator.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::OSCILLATOR {

constexpr Float MIX = 0.5f;

struct Module {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::VOICE::Allocator allocator;
  CORE::OSCILLATOR::Oscillator oscillator;
  CORE::OSCILLATOR::Super super;
  CORE::OSCILLATOR::Table table;
  Float pitch = 0;
  Flag stale = true;
  Flag sounding = false;
  CORE::BLOCK::Meter meter;
};

void seat(Module &module);
void settle(Module &module);
void tune(Module &module, Float pitch);
void apply(Module &module, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::OSCILLATOR
