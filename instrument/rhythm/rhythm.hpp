// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::RHYTHM {

enum Voice : Whole { KICK, SNARE, HAT, CYMBAL, VOICES };

constexpr Whole PATTERN = 0;
constexpr Whole TEMPO = 1;
constexpr Whole RUN = 2;
constexpr Whole FILL = 3;
constexpr Whole LEVELS = 4;

constexpr Whole PATTERNS = 16;

inline constexpr STRING::Hot NAMES[PATTERNS] = {
  "Rock",   "Disco", "Waltz", "Bossa",   "Samba",  "Rumba",
  "Tango",  "March", "Swing", "Shuffle", "Ballad", "Beguine",
  "Chacha", "Mambo", "Polka", "Foxtrot"};

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Pattern", "", 0, 0, PATTERNS - 1, PATTERNS - 1, NAMES},
  {"Tempo", "bpm", 120, 40, 240, 0, nullptr},
  {"Run", "", 1, 0, 1, 1, nullptr},
  {"Fill", "", 0, 0, 1, 1, nullptr},
  {"Kick Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Snare Level", "", 0.7f, 0, 1, 0, nullptr},
  {"Hat Level", "", 0.5f, 0, 1, 0, nullptr},
  {"Cymbal Level", "", 0.4f, 0, 1, 0, nullptr}};
inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
constexpr Whole PARAMETERS = SHEET.count;
static_assert(PARAMETERS == LEVELS + VOICES);

}  // namespace SOUND::PLUGINS::RHYTHM
