// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "envelope.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::PLUGINS::ENVELOPE {

static_assert(EXPONENTIAL == CORE::ENVELOPE::EXPONENTIAL);
static_assert(RESUME == CORE::ENVELOPE::RESUME);

constexpr Whole LEAD = 0;

struct Module {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::VOICE::Allocator allocator;
  CORE::ENVELOPE::Envelope envelope;
  CORE::ENVELOPE::Gate gate;
  CORE::BLOCK::Edge edge;
  CORE::BLOCK::Meter meter;
};

void seat(Module &module);
void settle(Module &module);
auto shaped(const Float *rows) -> CORE::ENVELOPE::Envelope;
void apply(Module &module, const AUDIO::PLUGIN::Event &event);
void strike(Module &module, Float velocity);
void lift(Module &module);
void follow(Module &module, AUDIO::PLUGIN::Sample *const *lanes, Whole frame);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::ENVELOPE

namespace SOUND::PLUGINS::ENVELOPE::IN {

constexpr Whole GATE = 0;
constexpr Whole CV = 1;

}  // namespace SOUND::PLUGINS::ENVELOPE::IN
