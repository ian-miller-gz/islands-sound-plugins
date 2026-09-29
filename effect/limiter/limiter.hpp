// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::LIMITER {

constexpr Whole CEILING = 0;
constexpr Whole RELEASE = 1;
constexpr Whole GAIN = 2;
constexpr Whole PARAMETERS = 3;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Ceiling", "dB", -1, -24, 0, 0, nullptr},
  {"Release", "ms", 100, 1, 1000, 0, nullptr},
  {"Gain", "dB", 0, 0, 24, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::LIMITER
