// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::KIT {

enum Drum : Whole {
  KICK,
  SNARE,
  HAT,
  CLAP,
  TOM,
  CYMBAL,
  COWBELL,
  CLAVE,
  CONGA,
  DRUMS
};

constexpr Whole TUNE = 0;
constexpr Whole LEVEL = 1;
constexpr Whole KNOBS = 2;
constexpr Whole ACCENT = DRUMS * KNOBS;
constexpr Whole DECAYS = ACCENT + 1;
constexpr Whole TONES = DECAYS + DRUMS;
constexpr Whole TONED = COWBELL;
constexpr Whole PANS = TONES + TONED;
constexpr Whole CURVE = PANS + DRUMS;

constexpr auto place(Whole drum, Whole knob) -> Whole {
  return drum * KNOBS + knob;
}

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Kick Tune", "st", 0, -12, 12, 0, nullptr},
  {"Kick Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Snare Tune", "st", 0, -12, 12, 0, nullptr},
  {"Snare Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Hat Tune", "st", 0, -12, 12, 0, nullptr},
  {"Hat Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Clap Tune", "st", 0, -12, 12, 0, nullptr},
  {"Clap Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Tom Tune", "st", 0, -12, 12, 0, nullptr},
  {"Tom Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Cymbal Tune", "st", 0, -12, 12, 0, nullptr},
  {"Cymbal Level", "", 0.6f, 0, 1, 0, nullptr},
  {"Cowbell Tune", "st", 0, -12, 12, 0, nullptr},
  {"Cowbell Level", "", 0.7f, 0, 1, 0, nullptr},
  {"Clave Tune", "st", 0, -12, 12, 0, nullptr},
  {"Clave Level", "", 0.7f, 0, 1, 0, nullptr},
  {"Conga Tune", "st", 0, -12, 12, 0, nullptr},
  {"Conga Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Accent", "", 0.5f, 0, 1, 0, nullptr},
  {"Kick Decay", "s", 0.5f, 0.05f, 3, 0, nullptr},
  {"Snare Decay", "s", 0.25f, 0.05f, 1.5f, 0, nullptr},
  {"Hat Decay", "s", 0.45f, 0.05f, 2, 0, nullptr},
  {"Clap Decay", "s", 0.3f, 0.05f, 2, 0, nullptr},
  {"Tom Decay", "s", 0.4f, 0.05f, 2, 0, nullptr},
  {"Cymbal Decay", "s", 1.5f, 0.2f, 5, 0, nullptr},
  {"Cowbell Decay", "s", 0.35f, 0.05f, 2, 0, nullptr},
  {"Clave Decay", "s", 0.06f, 0.01f, 0.5f, 0, nullptr},
  {"Conga Decay", "s", 0.3f, 0.05f, 1.5f, 0, nullptr},
  {"Kick Tone", "Hz", 4000, 200, 12000, 0, nullptr},
  {"Snare Tone", "Hz", 3000, 500, 10000, 0, nullptr},
  {"Hat Tone", "Hz", 6000, 2000, 14000, 0, nullptr},
  {"Clap Tone", "Hz", 1100, 400, 5000, 0, nullptr},
  {"Tom Tone", "Hz", 4000, 500, 12000, 0, nullptr},
  {"Cymbal Tone", "", 0.5f, 0, 1, 0, nullptr},
  {"Kick Pan", "", 0, -1, 1, 0, nullptr},
  {"Snare Pan", "", 0.1f, -1, 1, 0, nullptr},
  {"Hat Pan", "", -0.35f, -1, 1, 0, nullptr},
  {"Clap Pan", "", -0.1f, -1, 1, 0, nullptr},
  {"Tom Pan", "", 0.3f, -1, 1, 0, nullptr},
  {"Cymbal Pan", "", 0.45f, -1, 1, 0, nullptr},
  {"Cowbell Pan", "", -0.5f, -1, 1, 0, nullptr},
  {"Clave Pan", "", 0.5f, -1, 1, 0, nullptr},
  {"Conga Pan", "", -0.25f, -1, 1, 0, nullptr},
  {"Curve", "", 0, -1, 1, 0, nullptr}};
inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
constexpr Whole PARAMETERS = SHEET.count;
static_assert(PARAMETERS == CURVE + 1);

}  // namespace SOUND::PLUGINS::KIT
