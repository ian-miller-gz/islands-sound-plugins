// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::MACHINE {

enum Drum : Whole { KICK, SNARE, HAT, CLAP, TOM, DRUMS };

constexpr Whole TUNE = 0;
constexpr Whole LEVEL = 1;
constexpr Whole KNOBS = 2;
constexpr Whole ACCENT = DRUMS * KNOBS;

constexpr auto place(Whole drum, Whole knob) -> Whole {
  return drum * KNOBS + knob;
}

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Kick Tune", "st", 0, -12, 12, 0, nullptr},
  {"Kick Level", "", 0.9f, 0, 1, 0, nullptr},
  {"Snare Tune", "st", 0, -12, 12, 0, nullptr},
  {"Snare Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Hat Tune", "st", 0, -12, 12, 0, nullptr},
  {"Hat Level", "", 0.7f, 0, 1, 0, nullptr},
  {"Clap Tune", "st", 0, -12, 12, 0, nullptr},
  {"Clap Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Tom Tune", "st", 0, -12, 12, 0, nullptr},
  {"Tom Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Accent", "", 0.5f, 0, 1, 0, nullptr}};
inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
constexpr Whole PARAMETERS = SHEET.count;
static_assert(PARAMETERS == ACCENT + 1);

}  // namespace SOUND::MACHINE
