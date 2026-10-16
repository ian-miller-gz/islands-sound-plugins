// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <iterator>

#include "../../core/table/table.hpp"
#include "operator.indices.hpp"
#include "operator.rows.hpp"

namespace SOUND::PLUGINS::OPERATOR {

constexpr Float GRADES = 7;
constexpr Float BOTTOM = 21;
constexpr Float SUMMIT = 120;
constexpr Float MIDDLE = 60;
constexpr Float CENTRE = 50;
constexpr Float REACH = 48;
constexpr Float TRAVEL = 3;

inline constexpr STRING::Hot CURVES[] = {"Linear", "Exponential"};
inline constexpr STRING::Hot MODES[] = {"Poly", "Mono", "Legato"};

constexpr Whole FORMS = std::size(CURVES) - 1;
constexpr Whole VOICINGS = std::size(MODES) - 1;

inline constexpr STRING::Hot LABELS[OPERATORS][TRAITS] = {
  {"Sense1", "Fixed1", "Break1", "Lower1", "Upper1", "Curve1", "Tracking1"},
  {"Sense2", "Fixed2", "Break2", "Lower2", "Upper2", "Curve2", "Tracking2"},
  {"Sense3", "Fixed3", "Break3", "Lower3", "Upper3", "Curve3", "Tracking3"},
  {"Sense4", "Fixed4", "Break4", "Lower4", "Upper4", "Curve4", "Tracking4"},
  {"Sense5", "Fixed5", "Break5", "Lower5", "Upper5", "Curve5", "Tracking5"},
  {"Sense6", "Fixed6", "Break6", "Lower6", "Upper6", "Curve6", "Tracking6"}};

inline constexpr CORE::TABLE::Row TRAIT[] = {
  {"", "", GRADES, 0, GRADES, Whole(GRADES), nullptr},
  {"", "", 0, 0, 1, 1, nullptr},
  {"", "", MIDDLE, BOTTOM, SUMMIT, Whole(SUMMIT - BOTTOM), nullptr},
  {"", "", 0, -TOP, TOP, Whole(TOP) * 2, nullptr},
  {"", "", 0, -TOP, TOP, Whole(TOP) * 2, nullptr},
  {"", "", 0, 0, Float(FORMS), FORMS, CURVES},
  {"", "", 2, 0, GRADES, Whole(GRADES), nullptr}};
static_assert(std::size(TRAIT) == TRAITS);

inline constexpr Float UPPERS[OPERATORS] = {0, -30, 0, 0, 0, 0};

inline constexpr CORE::TABLE::Row CODA[] = {
  {"Sweep", "st", SEMITONES, 0, REACH, 0, nullptr},
  {"Pace1", "", TOP, 0, TOP, Whole(TOP), nullptr},
  {"Pace2", "", 55, 0, TOP, Whole(TOP), nullptr},
  {"Pace3", "", TOP, 0, TOP, Whole(TOP), nullptr},
  {"Pace4", "", TOP, 0, TOP, Whole(TOP), nullptr},
  {"Height1", "", 58, 0, TOP, Whole(TOP), nullptr},
  {"Height2", "", CENTRE, 0, TOP, Whole(TOP), nullptr},
  {"Height3", "", CENTRE, 0, TOP, Whole(TOP), nullptr},
  {"Height4", "", CENTRE, 0, TOP, Whole(TOP), nullptr},
  {"Mode", "", 0, 0, Float(VOICINGS), VOICINGS, MODES},
  {"Glide", "s", 0, 0, TRAVEL, 0, nullptr},
  {"Hold", "", 0, 0, 1, 1, nullptr}};
static_assert(std::size(CODA) == PARAMETERS - SWEEP);

constexpr auto extend(Whole index) -> CORE::TABLE::Row {
  if (index >= SWEEP) return CODA[index - SWEEP];
  const Whole unit = (index - SECOND) / TRAITS;
  const Whole at = (index - SECOND) % TRAITS;
  CORE::TABLE::Row row = TRAIT[at];
  row.name = LABELS[unit][at];
  if (at == UPPER) row.resting = UPPERS[unit];
  return row;
}

}  // namespace SOUND::PLUGINS::OPERATOR
