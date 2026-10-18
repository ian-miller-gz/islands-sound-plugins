// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "rhythm.hpp"
#include "../../core/block/block.hpp"
#include "../../core/clock/clock.hpp"
#include "../../core/noise/noise.hpp"
#include "../../core/percussion/percussion.hpp"

namespace SOUND::PLUGINS::RHYTHM {

constexpr Whole SIXTEENTHS = 4;
constexpr Whole TRIPLETS = 3;

struct Pattern {
  Whole beats = 0;
  Whole division = 0;
  Whole lanes[VOICES] = {};
  Whole accent = 0;
};

struct Pending {
  Whole left = 0;
  Whole part = 0;
  Float velocity = 0;
};

struct Box {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::PERCUSSION::Kick kick;
  CORE::PERCUSSION::Snare snare;
  CORE::PERCUSSION::Hat hat;
  CORE::PERCUSSION::Cymbal cymbal;
  CORE::PERCUSSION::Tom tom;
  CORE::PERCUSSION::Clap clap;
  CORE::PERCUSSION::Clave rim;
  CORE::PERCUSSION::Cowbell cowbell;
  CORE::CLOCK::Clock clock;
  CORE::NOISE::White white;
  Pending pendings[VOICES];
  CORE::BLOCK::Meter meter;
};

namespace STRAIGHT {

auto pattern(Whole at) -> const Pattern &;

}  // namespace STRAIGHT

namespace TRIPLE {

auto pattern(Whole at) -> const Pattern &;

}  // namespace TRIPLE

auto pattern(Whole index) -> const Pattern &;
auto length(const Pattern &pattern) -> Whole;
auto struck(const Pattern &pattern, Whole voice, Whole step) -> Flag;
auto accented(const Pattern &pattern, Whole step) -> Flag;

void settle(Box &box);
void pace(Box &box);
void apply(Box &box, const AUDIO::PLUGIN::Event &event);
void strike(Box &box, Whole voice, Whole part, Float velocity);
void queue(Box &box, Whole voice, Whole part, Float velocity);
void drain(Box &box);
void rewind(Box &box);

void pulse(Box &box);
auto mix(Box &box) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::RHYTHM
