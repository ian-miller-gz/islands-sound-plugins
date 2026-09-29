// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::FUZZ {

constexpr Whole FUZZ = 0;
constexpr Whole BIAS = 1;
constexpr Whole TONE = 2;
constexpr Whole LEVEL = 3;
constexpr Whole PARAMETERS = 4;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Fuzz", "", 0.6f, 0, 1, 0, nullptr},
  {"Bias", "", 0.2f, -0.5f, 0.5f, 0, nullptr},
  {"Tone", "", 0.5f, 0, 1, 0, nullptr},
  {"Level", "dB", -6, -24, 6, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::FUZZ
