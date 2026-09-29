// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::GRAPHIC {

constexpr Whole BANDS = 10;
constexpr Whole GAIN = 0;
constexpr Whole PARAMETERS = GAIN + BANDS;

constexpr Float CUT = -12;
constexpr Float BOOST = 12;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"31Hz", "dB", 0, CUT, BOOST, 0, nullptr},
  {"63Hz", "dB", 0, CUT, BOOST, 0, nullptr},
  {"125Hz", "dB", 0, CUT, BOOST, 0, nullptr},
  {"250Hz", "dB", 0, CUT, BOOST, 0, nullptr},
  {"500Hz", "dB", 0, CUT, BOOST, 0, nullptr},
  {"1kHz", "dB", 0, CUT, BOOST, 0, nullptr},
  {"2kHz", "dB", 0, CUT, BOOST, 0, nullptr},
  {"4kHz", "dB", 0, CUT, BOOST, 0, nullptr},
  {"8kHz", "dB", 0, CUT, BOOST, 0, nullptr},
  {"16kHz", "dB", 0, CUT, BOOST, 0, nullptr}};

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
static_assert(SHEET.count == PARAMETERS);

}  // namespace SOUND::PLUGINS::GRAPHIC
