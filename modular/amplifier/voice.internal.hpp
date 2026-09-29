// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "amplifier.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::PLUGINS::AMPLIFIER {

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

}  // namespace SOUND::PLUGINS::AMPLIFIER
