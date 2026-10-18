// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "choir.hpp"
#include "voice.seats.hpp"
#include "../../core/block/block.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::PLUGINS::CHOIR {

constexpr Whole NOTES = 6;
constexpr Whole CONTROL = 64;
constexpr Float CENT = 100.0f;

struct Choir {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  Float gain = 0;
  Float part = 0;
  Float pole = 0;
  Float sway = 0;
  Float scatter = 0;
  Whole singers = SEATS;
  Float blend = 0;
  CORE::BLOCK::Stride stride;
  CORE::MODULATOR::Seed seed = CORE::MODULATOR::SEED;
  CORE::OSCILLATOR::Table table;
  CORE::ENVELOPE::Envelope envelope;
  CORE::MODULATOR::Lfo motion;
  CORE::VOICE::Allocator allocator;
  Part parts[NOTES];
  Ensemble ensemble;
  Room room;
  CORE::BLOCK::Meter meter;
};

void build(Choir &choir);
void allocate(Choir &choir);
void settle(Choir &choir);
void settle(Ensemble &ensemble, Room &room, const Float *rows, Whole rate);
void apply(Choir &choir, const AUDIO::PLUGIN::Event &event);
void place(Choir &choir);
void tune(Choir &choir, CORE::VOICE::Note &note, Part &part, Flag leap);
auto cast(const Choir &choir, const CORE::VOICE::Note &note) -> Float;
void steer(Choir &choir);
auto sing(Choir &choir) -> Stereo;
auto spread(Choir &choir, Stereo dry) -> Stereo;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::CHOIR
