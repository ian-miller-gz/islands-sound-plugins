// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::AMPLIFIER {

constexpr Whole OFFSET = 0;
constexpr Whole DEPTH = 1;
constexpr Whole GAIN = 2;
constexpr Whole RESPONSE = 3;
constexpr Whole MODE = 4;
constexpr Whole PARAMETERS = 5;

constexpr Whole IN = 0;
constexpr Whole CV = 1;

enum Mode : Whole { CLAMP, RING, LOWPASS, MODES };

constexpr Float INVERTED = -1.0f;
constexpr Float LOUDEST = 2.0f;
constexpr Float HALF = 0.5f;
constexpr Float LAST = MODES - 1;

inline constexpr STRING::Hot WORDS[] = {"Clamp", "Ring", "Lowpass"};
static_assert(std::size(WORDS) == MODES);

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Offset", "", 0, 0, 1, 0, nullptr},
  {"Depth", "", 1, INVERTED, 1, 0, nullptr},
  {"Gain", "", 1, 0, LOUDEST, 0, nullptr},
  {"Response", "", HALF, 0, 1, 0, nullptr},
  {"Mode", "", 0, 0, LAST, MODES - 1, WORDS}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::PLUGINS::AMPLIFIER
