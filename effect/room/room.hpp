// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::ROOM {

constexpr Whole SIZE = 0;
constexpr Whole DECAY = 1;
constexpr Whole MIX = 2;
constexpr Whole PARAMETERS = 3;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Size", "", 0.5f, 0.25f, 2, 0, nullptr},
  {"Decay", "s", 0.8f, 0.1f, 5, 0, nullptr},
  {"Mix", "", 0.25f, 0, 1, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::ROOM
