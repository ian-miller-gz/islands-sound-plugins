// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::PADS {

enum Pad : Whole { KICK, SNARE, HIGH, MID, LOW, FLOOR, PADS };

enum Knob : Whole { TUNE, BEND, DECAY, TONE, NOISE, CLICK, LEVEL, KNOBS };

constexpr auto place(Whole pad, Whole knob) -> Whole {
  return pad * KNOBS + knob;
}

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Kick Tune", "Hz", 60, 30, 1000, 0, nullptr},
  {"Kick Bend", "", 0.6f, 0, 1, 0, nullptr},
  {"Kick Decay", "s", 0.45f, 0.05f, 2, 0, nullptr},
  {"Kick Tone", "Hz", 600, 200, 12000, 0, nullptr},
  {"Kick Noise", "", 0.1f, 0, 1, 0, nullptr},
  {"Kick Click", "", 0.5f, 0, 1, 0, nullptr},
  {"Kick Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Snare Tune", "Hz", 190, 30, 1000, 0, nullptr},
  {"Snare Bend", "", 0.3f, 0, 1, 0, nullptr},
  {"Snare Decay", "s", 0.3f, 0.05f, 2, 0, nullptr},
  {"Snare Tone", "Hz", 6000, 200, 12000, 0, nullptr},
  {"Snare Noise", "", 0.6f, 0, 1, 0, nullptr},
  {"Snare Click", "", 0.4f, 0, 1, 0, nullptr},
  {"Snare Level", "", 0.8f, 0, 1, 0, nullptr},
  {"High Tune", "Hz", 240, 30, 1000, 0, nullptr},
  {"High Bend", "", 0.5f, 0, 1, 0, nullptr},
  {"High Decay", "s", 0.4f, 0.05f, 2, 0, nullptr},
  {"High Tone", "Hz", 2500, 200, 12000, 0, nullptr},
  {"High Noise", "", 0.2f, 0, 1, 0, nullptr},
  {"High Click", "", 0.3f, 0, 1, 0, nullptr},
  {"High Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Mid Tune", "Hz", 180, 30, 1000, 0, nullptr},
  {"Mid Bend", "", 0.5f, 0, 1, 0, nullptr},
  {"Mid Decay", "s", 0.5f, 0.05f, 2, 0, nullptr},
  {"Mid Tone", "Hz", 2000, 200, 12000, 0, nullptr},
  {"Mid Noise", "", 0.2f, 0, 1, 0, nullptr},
  {"Mid Click", "", 0.3f, 0, 1, 0, nullptr},
  {"Mid Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Low Tune", "Hz", 130, 30, 1000, 0, nullptr},
  {"Low Bend", "", 0.5f, 0, 1, 0, nullptr},
  {"Low Decay", "s", 0.6f, 0.05f, 2, 0, nullptr},
  {"Low Tone", "Hz", 1600, 200, 12000, 0, nullptr},
  {"Low Noise", "", 0.2f, 0, 1, 0, nullptr},
  {"Low Click", "", 0.3f, 0, 1, 0, nullptr},
  {"Low Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Floor Tune", "Hz", 95, 30, 1000, 0, nullptr},
  {"Floor Bend", "", 0.5f, 0, 1, 0, nullptr},
  {"Floor Decay", "s", 0.8f, 0.05f, 2, 0, nullptr},
  {"Floor Tone", "Hz", 1200, 200, 12000, 0, nullptr},
  {"Floor Noise", "", 0.2f, 0, 1, 0, nullptr},
  {"Floor Click", "", 0.3f, 0, 1, 0, nullptr},
  {"Floor Level", "", 0.8f, 0, 1, 0, nullptr}};
inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
constexpr Whole PARAMETERS = SHEET.count;
static_assert(PARAMETERS == Whole(PADS) * KNOBS);

}  // namespace SOUND::PLUGINS::PADS
