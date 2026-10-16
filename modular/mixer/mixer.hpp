// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::MIXER {

constexpr Whole INS = 4;
constexpr Whole OUT = 0;
constexpr Whole CV = INS;
constexpr Whole LEVEL = 0;
constexpr Whole PAN = LEVEL + INS;
constexpr Whole GAIN = PAN + INS;
constexpr Whole MUTE = GAIN + 1;
constexpr Whole TAPER = MUTE + INS;
constexpr Whole AMOUNT = TAPER + 1;
constexpr Whole PARAMETERS = AMOUNT + 1;

constexpr Float LEFT = -1;
constexpr Float RIGHT = 1;
constexpr Float INVERTED = -1;
constexpr Float LOUDEST = 2;

enum Taper : Whole { LINEAR, LOGARITHMIC, TAPERS };

inline constexpr STRING::Hot SWITCHES[] = {"Off", "On"};
inline constexpr STRING::Hot CURVES[] = {"Linear", "Audio"};
static_assert(std::size(CURVES) == TAPERS);

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Level1", "", 1, INVERTED, 1, 0, nullptr},
  {"Level2", "", 1, INVERTED, 1, 0, nullptr},
  {"Level3", "", 1, INVERTED, 1, 0, nullptr},
  {"Level4", "", 1, INVERTED, 1, 0, nullptr},
  {"Pan1", "", 0, LEFT, RIGHT, 0, nullptr},
  {"Pan2", "", 0, LEFT, RIGHT, 0, nullptr},
  {"Pan3", "", 0, LEFT, RIGHT, 0, nullptr},
  {"Pan4", "", 0, LEFT, RIGHT, 0, nullptr},
  {"Gain", "", 1, 0, LOUDEST, 0, nullptr},
  {"Mute1", "", 0, 0, 1, 1, SWITCHES},
  {"Mute2", "", 0, 0, 1, 1, SWITCHES},
  {"Mute3", "", 0, 0, 1, 1, SWITCHES},
  {"Mute4", "", 0, 0, 1, 1, SWITCHES},
  {"Taper", "", LOGARITHMIC, 0, 1, 1, CURVES},
  {"Amount", "", 1, INVERTED, 1, 0, nullptr}};
static_assert(std::size(ROWS) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::PLUGINS::MIXER
