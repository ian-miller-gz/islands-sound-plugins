// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::AMPLIFIER {

constexpr Whole ATTACK = 0;
constexpr Whole DECAY = 1;
constexpr Whole SUSTAIN = 2;
constexpr Whole RELEASE = 3;
constexpr Whole VELOCITY = 4;
constexpr Whole GAIN = 5;
constexpr Whole PARAMETERS = 6;

constexpr Float QUICKEST = 0.001f;
constexpr Float SLOWEST = 10.0f;
constexpr Float LOUDEST = 2.0f;

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Attack", "s", 0.005f, QUICKEST, SLOWEST, 0, nullptr},
  {"Decay", "s", 0.2f, QUICKEST, SLOWEST, 0, nullptr},
  {"Sustain", "", 0.7f, 0, 1, 0, nullptr},
  {"Release", "s", 0.3f, QUICKEST, SLOWEST, 0, nullptr},
  {"Velocity", "", 0.5f, 0, 1, 0, nullptr},
  {"Gain", "", 1, 0, LOUDEST, 0, nullptr}};
static_assert(sizeof(ROWS) / sizeof(ROWS[0]) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::PLUGINS::AMPLIFIER
