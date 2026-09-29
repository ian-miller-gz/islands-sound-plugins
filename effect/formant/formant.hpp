// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::FORMANT {

constexpr Whole SHIFT = 0;
constexpr Whole PITCH = 1;
constexpr Whole MIX = 2;
constexpr Whole PARAMETERS = 3;

constexpr Float OCTAVE = 12.0f;
constexpr Float THIRD = 3.0f;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Formant", "st", THIRD, -OCTAVE, OCTAVE, 0, nullptr},
  {"Pitch", "st", 0, -OCTAVE, OCTAVE, 0, nullptr},
  {"Mix", "", 1, 0, 1, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::FORMANT
