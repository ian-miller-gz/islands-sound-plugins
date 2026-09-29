// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::OVERDRIVE {

constexpr Whole DRIVE = 0;
constexpr Whole TONE = 1;
constexpr Whole LEVEL = 2;
constexpr Whole PARAMETERS = 3;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Drive", "", 0.5f, 0, 1, 0, nullptr},
  {"Tone", "", 0.5f, 0, 1, 0, nullptr},
  {"Level", "dB", -6, -24, 6, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::OVERDRIVE
