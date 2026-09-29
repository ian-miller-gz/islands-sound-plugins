// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::HAT {

constexpr Whole TUNE = 0;
constexpr Whole CLOSED = 1;
constexpr Whole OPEN = 2;
constexpr Whole TONE = 3;
constexpr Whole LEVEL = 4;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tune", "st", 0, -12, 12, 0, nullptr},
  {"Closed", "s", 0.06f, 0.01f, 0.5f, 0, nullptr},
  {"Open", "s", 0.45f, 0.05f, 2, 0, nullptr},
  {"Tone", "Hz", 6000, 2000, 14000, 0, nullptr},
  {"Level", "", 0.8f, 0, 1, 0, nullptr}};
inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
constexpr Whole PARAMETERS = SHEET.count;

}  // namespace SOUND::HAT

namespace SOUND::HAT::PITCH {
constexpr Whole OPEN = 46;
}  // namespace SOUND::HAT::PITCH
