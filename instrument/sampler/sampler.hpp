// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"
#include "sampler.indices.hpp"

namespace SOUND::PLUGINS::SAMPLER {

constexpr Whole POLYPHONY = 16;
constexpr Float HIGHEST = 127;
constexpr Float DETUNE = 50;
constexpr Float PERCENT = 100;
constexpr Float BLEND = 50;
constexpr Float QUICKEST = 0.001f;
constexpr Float BRIEFEST = 0.005f;
constexpr Float SLOWEST = 4;
constexpr Float SEMITONES = 24;
constexpr Float LONGEST = 2;
constexpr Float LOWEST = 20;
constexpr Float BRIGHTEST = 20000;
constexpr Float OCTAVES = 4;
constexpr Float TOUCHES = 3;
constexpr Float SLOWER = 0.05f;
constexpr Float FASTEST = 20;
constexpr Float DEPTH = 2;

inline constexpr STRING::Hot CLIPS[] = {"Tone"};
inline constexpr STRING::Hot LOOPS[] = {
  "Off", "Forward", "Pingpong", "Sustain"};
inline constexpr STRING::Hot CURVES[] = {"Linear", "Exponential"};
inline constexpr STRING::Hot MODES[] = {"Poly", "Mono", "Legato"};
inline constexpr STRING::Hot FILTERS[] = {
  "Off", "Low", "Band", "High", "Notch"};
inline constexpr STRING::Hot QUALITIES[] = {"Linear", "Cubic", "Double"};

template <Whole COUNT>
constexpr auto steps(const STRING::Hot (&)[COUNT]) -> Whole {
  return COUNT - 1;
}

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Gain", "", 0.80f, 0, 1, 0, nullptr},
  {"Stock", "", 0, 0, 0, 0, CLIPS},
  {"Root", "", 60, 0, HIGHEST, 0, nullptr},
  {"Tune", "ct", 0, -DETUNE, DETUNE, 0, nullptr},
  {"Start", "%", 0, 0, PERCENT, 0, nullptr},
  {"Loop", "", 0, 0, Float(steps(LOOPS)), steps(LOOPS), LOOPS},
  {"Attack", "s", 0.01f, QUICKEST, SLOWEST, 0, nullptr},
  {"Release", "s", 0.20f, BRIEFEST, SLOWEST, 0, nullptr},
  {"Decay", "s", 1, QUICKEST, SLOWEST, 0, nullptr},
  {"Sustain", "", 1, 0, 1, 0, nullptr},
  {"Curve", "", 1, 0, 1, steps(CURVES), CURVES},
  {"End", "%", PERCENT, 0, PERCENT, 0, nullptr},
  {"Reverse", "", 0, 0, 1, 1, nullptr},
  {"Repeat", "%", 50, 0, PERCENT, 0, nullptr},
  {"Turn", "%", 95, 0, PERCENT, 0, nullptr},
  {"Fade", "%", 10, 0, BLEND, 0, nullptr},
  {"Coarse", "st", 0, -SEMITONES, SEMITONES, 0, nullptr},
  {"Track", "", 1, 0, 1, 1, nullptr},
  {"Mode", "", 0, 0, Float(steps(MODES)), steps(MODES), MODES},
  {"Voices", "", POLYPHONY, 1, POLYPHONY, POLYPHONY - 1, nullptr},
  {"Glide", "s", 0, 0, LONGEST, 0, nullptr},
  {"Velocity", "", 1, 0, 1, 0, nullptr},
  {"Filter", "", 0, 0, Float(steps(FILTERS)), steps(FILTERS), FILTERS},
  {"Cutoff", "Hz", 8000, LOWEST, BRIGHTEST, 0, nullptr},
  {"Resonance", "", 0.2f, 0, 1, 0, nullptr},
  {"Keyboard", "", 0, 0, 1, 0, nullptr},
  {"Envelope", "oct", 2, -OCTAVES, OCTAVES, 0, nullptr},
  {"Sweep", "s", 0.005f, QUICKEST, SLOWEST, 0, nullptr},
  {"Close", "s", 0.5f, QUICKEST, SLOWEST, 0, nullptr},
  {"Touch", "oct", 0, 0, TOUCHES, 0, nullptr},
  {"Drop", "st", 0, -SEMITONES, SEMITONES, 0, nullptr},
  {"Rate", "Hz", 5, SLOWER, FASTEST, 0, nullptr},
  {"Vibrato", "st", 0, 0, DEPTH, 0, nullptr},
  {"Wobble", "oct", 0, 0, OCTAVES, 0, nullptr},
  {"Quality", "", 2, 0, Float(steps(QUALITIES)), steps(QUALITIES), QUALITIES}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::PLUGINS::SAMPLER
