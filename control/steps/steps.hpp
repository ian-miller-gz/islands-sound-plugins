// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::STEPS {

constexpr Whole LENGTH = 16;
constexpr Whole SHEETS = 3;

constexpr Whole TEMPO = 0;
constexpr Whole DIVISION = 1;
constexpr Whole SWING = 2;
constexpr Whole RUN = 3;
constexpr Whole PITCH = 4;
constexpr Whole VELOCITY = PITCH + LENGTH;
constexpr Whole GATE = VELOCITY + LENGTH;
constexpr Whole PARAMETERS = GATE + LENGTH;

constexpr Float SLOWEST = 20.0f;
constexpr Float FASTEST = 300.0f;
constexpr Float EVEN = 50.0f;
constexpr Float LOOSEST = 75.0f;
constexpr Float HIGHEST = 127.0f;
constexpr Whole SIXTEENTH = 4;
constexpr Whole DIVISIONS = 7;

inline constexpr STRING::Hot LABELS[] = {"1/2",  "1/4",   "1/8", "1/8T",
                                         "1/16", "1/16T", "1/32"};
static_assert(sizeof(LABELS) / sizeof(LABELS[0]) == DIVISIONS);

inline constexpr Float BEATS[] = {0.5f, 1, 2, 3, 4, 6, 8};
static_assert(sizeof(BEATS) / sizeof(BEATS[0]) == DIVISIONS);

inline constexpr CORE::TABLE::Row HEAD[] = {
  {"Tempo", "bpm", 120, SLOWEST, FASTEST, 0, nullptr},
  {"Division", "", SIXTEENTH, 0, DIVISIONS - 1, DIVISIONS - 1, LABELS},
  {"Swing", "%", EVEN, EVEN, LOOSEST, 0, nullptr},
  {"Run", "", 1, 0, 1, 1, nullptr}};
static_assert(sizeof(HEAD) / sizeof(HEAD[0]) == PITCH);

inline constexpr STRING::Hot WORDS[SHEETS] = {"Pitch", "Velocity", "Gate"};

inline constexpr CORE::TABLE::Row SHAPES[SHEETS] = {
  {nullptr, "", 60, 0, HIGHEST, Whole(HIGHEST), nullptr},
  {nullptr, "", 0.8f, 0, 1, 0, nullptr},
  {nullptr, "", 0.5f, 0, 1, 0, nullptr}};

constexpr Whole WIDTH = 12;
constexpr Whole DECIMAL = 10;
constexpr char SPACE = ' ';
constexpr char ZERO = '0';

struct Names {
  char text[PARAMETERS][WIDTH] = {};
};

struct Built {
  CORE::TABLE::Row rows[PARAMETERS] = {};
};

constexpr void spell(STRING::Hot word, Whole number, char *into) {
  Whole at = 0;
  for (; word[at] != 0; ++at) into[at] = word[at];
  into[at++] = SPACE;
  if (number >= DECIMAL) into[at++] = char(ZERO + number / DECIMAL);
  into[at] = char(ZERO + number % DECIMAL);
}

constexpr auto named() -> Names {
  Names names;
  for (Whole at = PITCH; at < PARAMETERS; ++at)
    spell(
      WORDS[(at - PITCH) / LENGTH], (at - PITCH) % LENGTH + 1, names.text[at]);
  return names;
}

inline constexpr Names NAMES = named();

constexpr auto built() -> Built {
  Built built;
  for (Whole at = 0; at < PITCH; ++at) built.rows[at] = HEAD[at];
  for (Whole at = PITCH; at < PARAMETERS; ++at) {
    built.rows[at] = SHAPES[(at - PITCH) / LENGTH];
    built.rows[at].name = NAMES.text[at];
  }
  return built;
}

inline constexpr Built BUILT = built();

inline constexpr const CORE::TABLE::Row (&ROWS)[PARAMETERS] = BUILT.rows;

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::PLUGINS::STEPS
