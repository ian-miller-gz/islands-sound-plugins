// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::VOCODER {

constexpr Whole GAIN = 0;
constexpr Whole WAVE = 1;
constexpr Whole UNISON = 2;
constexpr Whole DETUNE = 3;
constexpr Whole LOW = 4;
constexpr Whole HIGH = 5;
constexpr Whole SHIFT = 6;
constexpr Whole ATTACK = 7;
constexpr Whole RELEASE = 8;
constexpr Whole UNVOICED = 9;
constexpr Whole DRY = 10;
constexpr Whole PARAMETERS = 11;

constexpr Whole SAW = 0;
constexpr Whole PULSE = 1;
constexpr Whole NOISE = 2;
constexpr Whole WAVES = 3;

constexpr Whole STACKS = 4;
constexpr Float LOUDEST = 2.0f;
constexpr Float WIDEST = 50.0f;
constexpr Float OCTAVE = 12.0f;
constexpr Float QUICKEST = 0.001f;
constexpr Float SLOWEST = 0.5f;

namespace RANGE {
constexpr Float LOWEST = 50.0f;
constexpr Float BOTTOM = 100.0f;
constexpr Float PARTING = 1000.0f;
constexpr Float SPLIT = 2000.0f;
constexpr Float TOP = 8000.0f;
constexpr Float HIGHEST = 12000.0f;
}  // namespace RANGE

inline constexpr STRING::Hot SHAPES[] = {"Saw", "Pulse", "Noise"};
static_assert(sizeof(SHAPES) / sizeof(SHAPES[0]) == WAVES);

inline constexpr STRING::Hot VOICES[] = {"1", "2", "3", "4"};
static_assert(sizeof(VOICES) / sizeof(VOICES[0]) == STACKS);

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Gain", "", 1.0f, 0, LOUDEST, 0, nullptr},
  {"Wave", "", Float(SAW), Float(SAW), Float(NOISE), WAVES - 1, SHAPES},
  {"Unison", "", 1, 1, Float(STACKS), STACKS - 1, VOICES},
  {"Detune", "ct", 10.0f, 0, WIDEST, 0, nullptr},
  {"Low", "Hz", RANGE::BOTTOM, RANGE::LOWEST, RANGE::PARTING, 0, nullptr},
  {"High", "Hz", RANGE::TOP, RANGE::SPLIT, RANGE::HIGHEST, 0, nullptr},
  {"Shift", "st", 0, -OCTAVE, OCTAVE, 0, nullptr},
  {"Attack", "s", 0.005f, QUICKEST, SLOWEST, 0, nullptr},
  {"Release", "s", 0.05f, QUICKEST, SLOWEST, 0, nullptr},
  {"Unvoiced", "", 0.5f, 0, 1, 0, nullptr},
  {"Dry", "", 0, 0, 1, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::VOCODER
