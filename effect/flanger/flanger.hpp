// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::FLANGER {

constexpr Whole FEEDBACK = 0;
constexpr Whole MANUAL = 1;
constexpr Whole RATE = 2;
constexpr Whole DEPTH = 3;
constexpr Whole POLARITY = 4;
constexpr Whole PARAMETERS = 5;

inline constexpr STRING::Hot SIGNS[] = {"Positive", "Negative"};

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Feedback", "", 0.5f, 0, 0.95f, 0, nullptr},
  {"Manual", "ms", 1, 0.1f, 10, 0, nullptr},
  {"Rate", "Hz", 0.25f, 0.02f, 5, 0, nullptr},
  {"Depth", "ms", 2, 0, 10, 0, nullptr},
  {"Polarity", "", 0, 0, 1, 1, SIGNS}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::FLANGER
