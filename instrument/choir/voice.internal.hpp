// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "choir.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/line/line.hpp"
#include "../../core/modulator/modulator.hpp"
#include "../../core/reverb/reverb.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::CHOIR {

constexpr Whole NOTES = 6;
constexpr Whole SINGERS = 8;
constexpr Whole CONTROL = 64;
constexpr Whole TAPS = 3;
constexpr Whole SIDES = CORE::REVERB::SIDES;
constexpr Float CENT = 100.0f;
constexpr Float BLEND = 0.35355339f;

using Stereo = CORE::REVERB::Stereo;

struct Seat {
  Float detune = 0;
  Float shift = 0;
  Float pan = 0;
  Float pace = 0;
};

struct Singer {
  CORE::GLOTTIS::Source source;
  CORE::FILTER::Formant formant;
  CORE::MODULATOR::Lfo vibrato;
  CORE::ENVELOPE::Gate gate;
  Seat seat;
  Float shift = 1;
  Stereo pan = {1, 1};
  Whole wait = 0;
  Flag pending = false;
};

struct Part {
  Singer singers[SINGERS];
};

struct Side {
  CORE::LINE::Line line;
  CORE::LINE::Sweep sweeps[TAPS];
};

struct Ensemble {
  Side sides[SIDES];
  CORE::MODULATOR::Lfo lfos[TAPS];
  Float mix = 0;
};

struct Room {
  CORE::REVERB::Network network;
  Float mix = 0;
};

struct Choir {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  Float gain = 0;
  Float place = 0;
  Float sway = 0;
  Float scatter = 0;
  Whole clock = 0;
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
void settle(Choir &choir);
void settle(Ensemble &ensemble, Room &room, const Float *rows, Whole rate);
void apply(Choir &choir, const AUDIO::PLUGIN::Event &event);
void place(Choir &choir);
void tune(const Choir &choir, const CORE::VOICE::Note &note, Part &part);
void steer(Choir &choir);
auto sing(Choir &choir) -> Stereo;
auto spread(Choir &choir, Stereo dry) -> Stereo;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::CHOIR
