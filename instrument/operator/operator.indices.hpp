// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"

namespace SOUND::PLUGINS::OPERATOR {

constexpr Whole OPERATORS = 6;
constexpr Whole STAGES = 4;

constexpr Whole RATIO = 0;
constexpr Whole DETUNE = 1;
constexpr Whole OUTPUT = 2;
constexpr Whole RATE = 3;
constexpr Whole LEVEL = RATE + STAGES;
constexpr Whole FIELDS = LEVEL + STAGES;

constexpr Whole ALGORITHM = 0;
constexpr Whole FEEDBACK = 1;
constexpr Whole FIRST = 2;
constexpr Whole TAIL = FIRST + OPERATORS * FIELDS;

constexpr Whole SPEED = TAIL;
constexpr Whole DELAY = TAIL + 1;
constexpr Whole WAVE = TAIL + 2;
constexpr Whole PITCH = TAIL + 3;
constexpr Whole AMPLITUDE = TAIL + 4;
constexpr Whole SYNC = TAIL + 5;
constexpr Whole VELOCITY = TAIL + 6;
constexpr Whole TUNE = TAIL + 7;
constexpr Whole BEND = TAIL + 8;
constexpr Whole VOLUME = TAIL + 9;
constexpr Whole PARAMETERS = TAIL + 10;

constexpr auto field(Whole unit, Whole at) -> Whole {
  return FIRST + unit * FIELDS + at;
}

}  // namespace SOUND::PLUGINS::OPERATOR
