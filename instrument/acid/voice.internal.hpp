// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "acid.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/filter/filter.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/oscillator/oscillator.hpp"
#include "../../core/shaper/shaper.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::PLUGINS::ACID {

enum Accent : Whole { PLAIN, STRONG, ACCENTS };

struct Synth {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::VOICE::Allocator allocator = {
    .count = 1,
    .mode = CORE::VOICE::MONO,
    .priority = CORE::VOICE::LAST,
    .legato = true};
  CORE::OSCILLATOR::Oscillator oscillator;
  CORE::FILTER::Pole coupling;
  CORE::FILTER::Diode diode;
  CORE::ENVELOPE::Envelope contours[ACCENTS];
  CORE::ENVELOPE::Envelope amplifier;
  CORE::ENVELOPE::Gate contour;
  CORE::ENVELOPE::Gate door;
  CORE::MODULATOR::Smoother push;
  CORE::SHAPER::Blocker blocker;
  Whole wave = CORE::OSCILLATOR::SAW;
  Whole accent = PLAIN;
  CORE::BLOCK::Meter meter;
};

void settle(Synth &synth);

void apply(Synth &synth, const AUDIO::PLUGIN::Event &event);

auto sound(Synth &synth) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::ACID
