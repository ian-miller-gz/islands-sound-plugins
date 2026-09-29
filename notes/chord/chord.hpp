// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::CHORD {

constexpr Whole KIND = 0;
constexpr Whole INVERSION = 1;
constexpr Whole SPREAD = 2;
constexpr Whole PARAMETERS = 3;

constexpr Whole KINDS = 5;
constexpr Whole INVERSIONS = 4;
constexpr Whole SPREADS = 4;
constexpr Whole TONES = 4;
constexpr Whole DOUBLES = 2;
constexpr Whole VOICES = TONES + DOUBLES;

constexpr Whole DOWN = 1;
constexpr Whole UP = 2;

inline constexpr STRING::Hot NAMES[] = {
  "Major", "Minor", "Seventh", "Sus", "Power"};
static_assert(sizeof(NAMES) / sizeof(NAMES[0]) == KINDS);

inline constexpr STRING::Hot POSITIONS[] = {"Root", "First", "Second", "Third"};
static_assert(sizeof(POSITIONS) / sizeof(POSITIONS[0]) == INVERSIONS);

inline constexpr STRING::Hot WIDTHS[] = {"Off", "Down", "Up", "Both"};
static_assert(sizeof(WIDTHS) / sizeof(WIDTHS[0]) == SPREADS);

struct Shape {
  Whole count = 0;
  Whole steps[TONES] = {};
};

inline constexpr Shape SHAPES[] = {
  {3, {0, 4, 7}},
  {3, {0, 3, 7}},
  {4, {0, 4, 7, 10}},
  {3, {0, 5, 7}},
  {2, {0, 7}}};
static_assert(sizeof(SHAPES) / sizeof(SHAPES[0]) == KINDS);

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Kind", "", 0, 0, KINDS - 1, KINDS - 1, NAMES},
  {"Inversion", "", 0, 0, INVERSIONS - 1, INVERSIONS - 1, POSITIONS},
  {"Spread", "", 0, 0, SPREADS - 1, SPREADS - 1, WIDTHS}};
static_assert(sizeof(ROWS) / sizeof(ROWS[0]) == PARAMETERS);

inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);

}  // namespace SOUND::CHORD
