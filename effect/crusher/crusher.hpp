// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::CRUSHER {

constexpr Whole BITS = 0;
constexpr Whole RATE = 1;
constexpr Whole JITTER = 2;
constexpr Whole MIX = 3;
constexpr Whole PARAMETERS = 4;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Bits", "", 8, 1, 16, 0, nullptr},
  {"Rate", "Hz", 8000, 100, 48000, 0, nullptr},
  {"Jitter", "", 0, 0, 1, 0, nullptr},
  {"Mix", "", 1, 0, 1, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::CRUSHER
