// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::RESONATOR {

constexpr Whole MODE = 0;
constexpr Whole CUTOFF = 1;
constexpr Whole EMPHASIS = 2;
constexpr Whole RATE = 3;
constexpr Whole DEPTH = 4;
constexpr Whole PARAMETERS = 5;

constexpr Whole LOW = 0;
constexpr Whole BAND = 1;
constexpr Whole HIGH = 2;
constexpr Whole NOTCH = 3;
constexpr Whole MODES = 4;

constexpr Float LOWEST = 20.0f;
constexpr Float HIGHEST = 20000.0f;
constexpr Float SLOWEST = 0.01f;
constexpr Float FASTEST = 20.0f;
constexpr Float OCTAVES = 4.0f;

inline constexpr STRING::Hot LABELS[] = {"Low", "Band", "High", "Notch"};
static_assert(sizeof(LABELS) / sizeof(LABELS[0]) == MODES);

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Mode", "", LOW, LOW, NOTCH, NOTCH, LABELS},
  {"Cutoff", "Hz", 1000, LOWEST, HIGHEST, 0, nullptr},
  {"Emphasis", "", 0.3f, 0, 1, 0, nullptr},
  {"Rate", "Hz", 0.5f, SLOWEST, FASTEST, 0, nullptr},
  {"Depth", "oct", 0.5f, 0, OCTAVES, 0, nullptr}};
static_assert(sizeof(ROWS) / sizeof(ROWS[0]) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::RESONATOR
