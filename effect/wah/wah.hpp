// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::WAH {

constexpr Whole SENSITIVITY = 0;
constexpr Whole RANGE = 1;
constexpr Whole EMPHASIS = 2;
constexpr Whole ATTACK = 3;
constexpr Whole RELEASE = 4;
constexpr Whole PARAMETERS = 5;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Sensitivity", "dB", 18, 0, 40, 0, nullptr},
  {"Range", "Hz", 2200, 500, 8000, 0, nullptr},
  {"Emphasis", "", 0.8f, 0, 0.95f, 0, nullptr},
  {"Attack", "ms", 5, 0.1f, 200, 0, nullptr},
  {"Release", "ms", 150, 5, 2000, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::WAH
