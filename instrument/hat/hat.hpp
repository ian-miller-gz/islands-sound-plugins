// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::HAT {

constexpr Whole TUNE = 0;
constexpr Whole CLOSED = 1;
constexpr Whole OPEN = 2;
constexpr Whole TONE = 3;
constexpr Whole LEVEL = 4;
constexpr Whole PEDAL = 5;
constexpr Whole NOISE = 6;
constexpr Whole METAL = 7;
constexpr Whole CURVE = 8;
constexpr Whole TOUCH = 9;

inline constexpr STRING::Hot METALS[] = {"Squares", "Rings"};

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Tune", "st", 0, -12, 12, 0, nullptr},
  {"Closed", "s", 0.06f, 0.01f, 0.5f, 0, nullptr},
  {"Open", "s", 0.45f, 0.05f, 2, 0, nullptr},
  {"Tone", "Hz", 6000, 2000, 14000, 0, nullptr},
  {"Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Pedal", "s", 0.04f, 0.01f, 0.2f, 0, nullptr},
  {"Noise", "", 0.15f, 0, 1, 0, nullptr},
  {"Metal", "", 0, 0, 1, 1, METALS},
  {"Curve", "", 0, -1, 1, 0, nullptr},
  {"Touch", "oct", 1, 0, 3, 0, nullptr}};
inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
constexpr Whole PARAMETERS = SHEET.count;

}  // namespace SOUND::PLUGINS::HAT

namespace SOUND::PLUGINS::HAT::PITCH {
constexpr Whole CLOSED = 42;
constexpr Whole PEDAL = 44;
constexpr Whole OPEN = 46;
}  // namespace SOUND::PLUGINS::HAT::PITCH
