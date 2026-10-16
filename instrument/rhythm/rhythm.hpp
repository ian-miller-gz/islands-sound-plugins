// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::RHYTHM {

enum Voice : Whole {
  KICK,
  SNARE,
  HAT,
  CYMBAL,
  TOM,
  CLAP,
  RIM,
  COWBELL,
  VOICES
};

constexpr Whole PATTERN = 0;
constexpr Whole TEMPO = 1;
constexpr Whole RUN = 2;
constexpr Whole FILL = 3;
constexpr Whole LEVELS = 4;

constexpr Whole SWING = 8;
constexpr Whole TIMING = 9;
constexpr Whole DYNAMICS = 10;
constexpr Whole ACCENT = 11;
constexpr Whole JOINED = 12;

inline constexpr Whole LEVELLED[VOICES] = {
  LEVELS + KICK,      LEVELS + SNARE,        LEVELS + HAT,
  LEVELS + CYMBAL,    JOINED + TOM - TOM,    JOINED + CLAP - TOM,
  JOINED + RIM - TOM, JOINED + COWBELL - TOM};

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
  {"Cymbal Level", "", 0.4f, 0, 1, 0, nullptr},
  {"Swing", "", 0, 0, 0.5f, 0, nullptr},
  {"Timing", "ms", 3, 0, 20, 0, nullptr},
  {"Dynamics", "", 0.15f, 0, 1, 0, nullptr},
  {"Accent", "", 0.5f, 0, 1, 0, nullptr},
  {"Tom Level", "", 0.6f, 0, 1, 0, nullptr},
  {"Clap Level", "", 0.6f, 0, 1, 0, nullptr},
  {"Rim Level", "", 0.5f, 0, 1, 0, nullptr},
  {"Cowbell Level", "", 0.4f, 0, 1, 0, nullptr}};
inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
constexpr Whole PARAMETERS = SHEET.count;
static_assert(PARAMETERS == JOINED + VOICES - TOM);

}  // namespace SOUND::PLUGINS::RHYTHM
