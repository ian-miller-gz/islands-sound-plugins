// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::DISTORTION {

constexpr Whole DRIVE = 0;
constexpr Whole TONE = 1;
constexpr Whole LEVEL = 2;
constexpr Whole MODE = 3;
constexpr Whole PARAMETERS = 4;

enum Mode : Whole { HARD, TUBE, MODES };

inline constexpr STRING::Hot CURVES[] = {"Hard", "Tube"};
static_assert(sizeof(CURVES) / sizeof(CURVES[0]) == MODES);

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Drive", "", 0.5f, 0, 1, 0, nullptr},
  {"Tone", "", 0.5f, 0, 1, 0, nullptr},
  {"Level", "dB", -6, -24, 6, 0, nullptr},
  {"Mode", "", HARD, HARD, TUBE, MODES - 1, CURVES}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::DISTORTION
