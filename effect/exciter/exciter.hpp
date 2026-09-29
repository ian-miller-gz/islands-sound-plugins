// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::EXCITER {

constexpr Whole FREQUENCY = 0;
constexpr Whole DRIVE = 1;
constexpr Whole MIX = 2;
constexpr Whole PARAMETERS = 3;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Frequency", "Hz", 3000, 1000, 10000, 0, nullptr},
  {"Drive", "dB", 12, 0, 24, 0, nullptr},
  {"Mix", "", 0.3f, 0, 1, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::EXCITER
