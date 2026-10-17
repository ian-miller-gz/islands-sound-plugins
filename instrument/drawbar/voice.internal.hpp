// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "drawbar.hpp"
#include "voice.parts.hpp"
#include "../../core/block/block.hpp"
#include "../../core/envelope/envelope.hpp"
#include "../../core/noise/noise.hpp"
#include "../../core/oscillator/oscillator.hpp"

namespace SOUND::PLUGINS::DRAWBAR {

struct Organ {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::OSCILLATOR::Table sine;
  CORE::PHASE::Wheel phases[WHEELS] = {};
  CORE::PHASE::Wheel steps[WHEELS] = {};
  Float targets[WHEELS] = {};
  Float levels[WHEELS] = {};
  Float strikes[WHEELS] = {};
  Float gains[BARS] = {};
  Flag keys[CORE::PHASE::PITCHES] = {};
  Whole held = 0;
  Float pole = 1;
  Float accent = 0;
  CORE::ENVELOPE::Envelope percussion;
  CORE::ENVELOPE::Gate gate;
  CORE::NOISE::Burst click;
  Contacts contacts;
  Leaks leaks;
  Preamp preamp;
  Room room;
  Scanner scanner;
  Rotary rotary;
  CORE::BLOCK::Meter meter;
};

void tune(Organ &organ);
void pull(Organ &organ);
void settle(Organ &organ);
void apply(Organ &organ, const AUDIO::PLUGIN::Event &event);

void arm(Organ &organ, Whole pitch);
void lift(Organ &organ, Whole pitch);
void close(Organ &organ);
auto closed(const Organ &organ, Whole pitch, Whole bar) -> Flag;

void build(Leaks &leaks);
void leak(Organ &organ);

void build(Scanner &scanner, Whole rate);
void settle(Scanner &scanner, Whole mode, Whole rate);
auto tick(Scanner &scanner, Float in) -> Float;

void build(Rotary &rotary, Whole rate);
void settle(Rotary &rotary, Whole mode, Whole rate);
auto tick(Rotary &rotary, const Organ &organ, Float in) -> Pair;

void build(Preamp &preamp, Whole rate);
auto tick(Preamp &preamp, const Float *rows, Float in) -> Float;

void build(Room &room, Whole rate);
auto tick(Room &room, Float wet, Pair dry) -> Pair;

auto sound(Organ &organ) -> Pair;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::DRAWBAR
