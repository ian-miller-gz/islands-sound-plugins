// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::LOFI {

constexpr Whole RATE = 0;
constexpr Whole BITS = 1;
constexpr Whole WOW = 2;
constexpr Whole NOISE = 3;
constexpr Whole BAND = 4;
constexpr Whole PARAMETERS = 5;

constexpr Float DEEPEST = 50;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Rate", "Hz", 11025, 1000, 48000, 0, nullptr},
  {"Bits", "", 10, 4, 16, 0, nullptr},
  {"Wow", "ct", 8, 0, DEEPEST, 0, nullptr},
  {"Noise", "dB", -54, -96, -24, 0, nullptr},
  {"Band", "Hz", 6000, 1000, 20000, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::LOFI
