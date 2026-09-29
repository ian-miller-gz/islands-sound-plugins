// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "envelope.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::PLUGINS::ENVELOPE {

static_assert(EXPONENTIAL == CORE::ENVELOPE::EXPONENTIAL);
static_assert(RESUME == CORE::ENVELOPE::RESUME);

struct Module {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::VOICE::Allocator allocator;
  CORE::ENVELOPE::Envelope envelope;
  CORE::ENVELOPE::Gate gate;
  CORE::BLOCK::Meter meter;
};

void seat(Module &module);
void settle(Module &module);
void apply(Module &module, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::ENVELOPE
