// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLATE {

constexpr Whole DECAY = 0;
constexpr Whole DAMP = 1;
constexpr Whole DIFFUSION = 2;
constexpr Whole PREDELAY = 3;
constexpr Whole MIX = 4;
constexpr Whole PARAMETERS = 5;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Decay", "", 0.5f, 0, 0.97f, 0, nullptr},
  {"Damp", "", 0.0005f, 0, 0.95f, 0, nullptr},
  {"Diffusion", "", 1, 0, 1, 0, nullptr},
  {"Predelay", "ms", 10, 0, 200, 0, nullptr},
  {"Mix", "", 0.3f, 0, 1, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLATE
