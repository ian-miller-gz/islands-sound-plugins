// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::ECHO {

constexpr Whole TIME = 0;
constexpr Whole FEEDBACK = 1;
constexpr Whole WOW = 2;
constexpr Whole FLUTTER = 3;
constexpr Whole SATURATION = 4;
constexpr Whole DAMP = 5;
constexpr Whole MIX = 6;
constexpr Whole PARAMETERS = 7;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Time", "ms", 350, 10, 1000, 0, nullptr},
  {"Feedback", "", 0.45f, 0, 1.1f, 0, nullptr},
  {"Wow", "", 0.25f, 0, 1, 0, nullptr},
  {"Flutter", "", 0.25f, 0, 1, 0, nullptr},
  {"Saturation", "", 0.3f, 0, 1, 0, nullptr},
  {"Damp", "", 0.4f, 0, 1, 0, nullptr},
  {"Mix", "", 0.35f, 0, 1, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::ECHO
