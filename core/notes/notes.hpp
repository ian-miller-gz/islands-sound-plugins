// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../noise/noise.hpp"
#include "../phase/phase.hpp"

namespace SOUND::PLUGINS::CORE::NOTES {

constexpr Whole KEYS = PHASE::PITCHES;
constexpr Whole HIGHEST = KEYS - 1;
constexpr Whole SILENT = KEYS;
constexpr Whole OCTAVE = 12;

struct Out {
  AUDIO::PLUGIN::Event *events = nullptr;
  Whole room = 0;
  Whole written = 0;
};

struct Tally {
  Whole counts[KEYS] = {};
};

auto struck(const AUDIO::PLUGIN::Event &event) -> Flag;
auto lifted(const AUDIO::PLUGIN::Event &event) -> Flag;
auto placed(Integer pitch) -> Whole;

auto put(Out &out, const AUDIO::PLUGIN::Event &event) -> Flag;
auto drain(Out &held, AUDIO::PLUGIN::Event *out, Whole room) -> Whole;

void strike(Tally &tally, Out &out, Whole pitch, Whole offset, Float velocity);
void lift(Tally &tally, Out &out, Whole pitch, Whole offset);

void sow(NOISE::White &white, Float seed);
auto draw(NOISE::White &white) -> Float;

}  // namespace SOUND::PLUGINS::CORE::NOTES
