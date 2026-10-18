// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::PERCUSSIVE {

enum Slot : Whole { KICK, SNARE, CLOSED, OPEN, TOM, SLOTS };

enum Knob : Whole { LEVEL, DECAY, KNOBS };

constexpr Whole GAIN = Whole(SLOTS) * KNOBS;
constexpr Whole CHOKE = GAIN + 1;
constexpr Whole TUNES = CHOKE + 1;
constexpr Whole BENDS = TUNES + SLOTS;
constexpr Whole TONES = BENDS + SLOTS;
constexpr Whole NOISES = TONES + SLOTS;
constexpr Whole CURVE = NOISES + SLOTS;

constexpr auto place(Whole slot, Whole knob) -> Whole {
  return slot * KNOBS + knob;
}

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Kick Level", "", 0.9f, 0, 1, 0, nullptr},
  {"Kick Decay", "s", 0.36f, 0.01f, 2, 0, nullptr},
  {"Snare Level", "", 0.75f, 0, 1, 0, nullptr},
  {"Snare Decay", "s", 0.17f, 0.01f, 2, 0, nullptr},
  {"Closed Level", "", 0.6f, 0, 1, 0, nullptr},
  {"Closed Decay", "s", 0.05f, 0.01f, 2, 0, nullptr},
  {"Open Level", "", 0.55f, 0, 1, 0, nullptr},
  {"Open Decay", "s", 0.38f, 0.01f, 2, 0, nullptr},
  {"Tom Level", "", 0.7f, 0, 1, 0, nullptr},
  {"Tom Decay", "s", 0.31f, 0.01f, 2, 0, nullptr},
  {"Gain", "", 0.8f, 0, 1, 0, nullptr},
  {"Choke", "", 1, 0, 1, 1, nullptr},
  {"Kick Tune", "Hz", 42, 20, 1000, 0, nullptr},
  {"Snare Tune", "Hz", 172, 20, 1000, 0, nullptr},
  {"Closed Tune", "Hz", 400, 20, 1000, 0, nullptr},
  {"Open Tune", "Hz", 400, 20, 1000, 0, nullptr},
  {"Tom Tune", "Hz", 84, 20, 1000, 0, nullptr},
  {"Kick Bend", "", 0.21f, 0, 1, 0, nullptr},
  {"Snare Bend", "", 0.05f, 0, 1, 0, nullptr},
  {"Closed Bend", "", 0, 0, 1, 0, nullptr},
  {"Open Bend", "", 0, 0, 1, 0, nullptr},
  {"Tom Bend", "", 0.17f, 0, 1, 0, nullptr},
  {"Kick Tone", "Hz", 2000, 200, 16000, 0, nullptr},
  {"Snare Tone", "Hz", 6000, 200, 16000, 0, nullptr},
  {"Closed Tone", "Hz", 12000, 200, 16000, 0, nullptr},
  {"Open Tone", "Hz", 12000, 200, 16000, 0, nullptr},
  {"Tom Tone", "Hz", 3000, 200, 16000, 0, nullptr},
  {"Kick Noise", "", 0, 0, 1, 0, nullptr},
  {"Snare Noise", "", 0.55f, 0, 1, 0, nullptr},
  {"Closed Noise", "", 1, 0, 1, 0, nullptr},
  {"Open Noise", "", 1, 0, 1, 0, nullptr},
  {"Tom Noise", "", 0, 0, 1, 0, nullptr},
  {"Curve", "", 0, -1, 1, 0, nullptr}};
inline constexpr CORE::TABLE::Sheet SHEET = CORE::TABLE::sheet(ROWS);
constexpr Whole PARAMETERS = SHEET.count;
static_assert(PARAMETERS == CURVE + 1);

}  // namespace SOUND::PLUGINS::PERCUSSIVE
