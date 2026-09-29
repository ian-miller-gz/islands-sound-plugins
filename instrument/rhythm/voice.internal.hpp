// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "rhythm.hpp"
#include "../../core/block/block.hpp"
#include "../../core/clock/clock.hpp"
#include "../../core/percussion/percussion.hpp"

namespace SOUND::RHYTHM {

struct Pattern {
  Whole beats = 0;
  Whole division = 0;
  Whole lanes[VOICES] = {};
};

struct Box {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::PERCUSSION::Kick kick;
  CORE::PERCUSSION::Snare snare;
  CORE::PERCUSSION::Hat hat;
  CORE::PERCUSSION::Cymbal cymbal;
  CORE::CLOCK::Clock clock;
  CORE::BLOCK::Meter meter;
};

auto pattern(Whole index) -> const Pattern &;
auto length(const Pattern &pattern) -> Whole;
auto struck(const Pattern &pattern, Whole voice, Whole step) -> Flag;

void settle(Box &box);
void pace(Box &box);
void apply(Box &box, const AUDIO::PLUGIN::Event &event);
void strike(Box &box, Whole voice, Whole part, Float velocity);

void pulse(Box &box);
auto mix(Box &box) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::RHYTHM
