// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "oscillator.hpp"
#include "../../core/block/block.hpp"
#include "../../core/oscillator/oscillator.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::PLUGINS::OSCILLATOR {

constexpr Float MIX = 0.5f;
constexpr Whole LEAD = 0;

struct Ins {
  Float pitch = 0;
  Float fm = 0;
};

struct Module {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::VOICE::Allocator allocator;
  CORE::OSCILLATOR::Oscillator oscillator;
  CORE::OSCILLATOR::Super super;
  CORE::OSCILLATOR::Table table;
  Float pitch = 0;
  Float fm = 0;
  Flag stale = true;
  Flag sounding = false;
  CORE::BLOCK::Meter meter;
};

void seat(Module &module);
void settle(Module &module);
void tune(Module &module, Float pitch, Float fm);
auto read(Module &module, AUDIO::PLUGIN::Sample *const *lanes, Whole frame)
  -> Ins;
auto pitched(Module &module, const Ins &ins) -> Float;
auto modulated(const Module &module, Float pitch, Float fm) -> Float;
void apply(Module &module, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::OSCILLATOR

namespace SOUND::PLUGINS::OSCILLATOR::IN {

constexpr Whole PITCH = 0;
constexpr Whole FM = 1;
constexpr Whole SYNC = 2;

}  // namespace SOUND::PLUGINS::OSCILLATOR::IN
