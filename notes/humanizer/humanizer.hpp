// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::HUMANIZER {

constexpr Whole VELOCITY = 0;
constexpr Whole TIMING = 1;
constexpr Whole PROBABILITY = 2;
constexpr Whole SEED = 3;
constexpr Whole PARAMETERS = 4;

constexpr Float LATEST = 4800.0f;
constexpr Float PERCENT = 100.0f;
constexpr Whole SEEDS = 999;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Velocity", "", 0.1f, 0, 1, 0, nullptr},
  {"Timing", "smp", 240, 0, LATEST, 0, nullptr},
  {"Probability", "%", PERCENT, 0, PERCENT, 0, nullptr},
  {"Seed", "", 0, 0, SEEDS, SEEDS, nullptr}};
static_assert(sizeof(ROWS) / sizeof(ROWS[0]) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::PLUGINS::HUMANIZER
