// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::CLAVE {

constexpr Whole TUNE = 0;
constexpr Whole DECAY = 1;
constexpr Whole LEVEL = 2;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tune", "Hz", 2500, 1000, 5000, 0, nullptr},
  {"Decay", "s", 0.06f, 0.01f, 0.5f, 0, nullptr},
  {"Level", "", 0.8f, 0, 1, 0, nullptr}};
inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
constexpr Whole PARAMETERS = SHEET.count;

}  // namespace SOUND::CLAVE
