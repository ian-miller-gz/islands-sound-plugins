// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "clave.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/percussion/percussion.hpp"
#include "../../core/phase/phase.hpp"

namespace SOUND::PLUGINS::CLAVE {

struct Voice {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  Float ratio = 1;
  CORE::PERCUSSION::Clave clave;
  CORE::BLOCK::Meter meter;
};

auto shifted(Float semitones) -> Float;
auto keyed(const Voice &voice) -> Flag;

void settle(Voice &voice);

void strike(Voice &voice, const AUDIO::PLUGIN::Event &event);

void apply(Voice &voice, const AUDIO::PLUGIN::Event &event);

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::CLAVE
