// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::RESONATOR {

constexpr Whole MODE = 0;
constexpr Whole CUTOFF = 1;
constexpr Whole EMPHASIS = 2;
constexpr Whole RATE = 3;
constexpr Whole SWEEP = 4;
constexpr Whole DEPTH = 5;
constexpr Whole MODEL = 6;
constexpr Whole DRIVE = 7;
constexpr Whole PARAMETERS = 8;

constexpr Whole IN = 0;
constexpr Whole CV = 1;

constexpr Whole LOW = 0;
constexpr Whole BAND = 1;
constexpr Whole HIGH = 2;
constexpr Whole NOTCH = 3;
constexpr Whole MODES = 4;

constexpr Whole VARIABLE = 0;
constexpr Whole LADDER = 1;
constexpr Whole DIODE = 2;
constexpr Whole SALLEN = 3;
constexpr Whole MODELS = 4;

constexpr Float LOWEST = 20.0f;
constexpr Float HIGHEST = 20000.0f;
constexpr Float SLOWEST = 0.01f;
constexpr Float FASTEST = 20.0f;
constexpr Float OCTAVES = 4.0f;
constexpr Float RANGE = 10.0f;
constexpr Float GRIT = 0.25f;

inline constexpr STRING::Hot LABELS[] = {"Low", "Band", "High", "Notch"};
static_assert(std::size(LABELS) == MODES);

inline constexpr STRING::Hot WORDS[] = {
  "Variable", "Ladder", "Diode", "Sallen"};
static_assert(std::size(WORDS) == MODELS);

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Mode", "", LOW, LOW, NOTCH, NOTCH, LABELS},
  {"Cutoff", "Hz", 1000, LOWEST, HIGHEST, 0, nullptr},
  {"Emphasis", "", 0.3f, 0, 1, 0, nullptr},
  {"Rate", "Hz", 0.5f, SLOWEST, FASTEST, 0, nullptr},
  {"Sweep", "oct", 0.5f, 0, OCTAVES, 0, nullptr},
  {"Depth", "oct", 0, -RANGE, RANGE, 0, nullptr},
  {"Model", "", VARIABLE, VARIABLE, SALLEN, SALLEN, WORDS},
  {"Drive", "", GRIT, 0, 1, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::PLUGINS::RESONATOR
