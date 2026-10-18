// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../core/table/table.hpp"

namespace SOUND::PLUGINS::MACHINE {

inline constexpr CORE::TABLE::Row ROWS[] = {
  {"Kick Tune", "st", 0, -12, 12, 0, nullptr},
  {"Kick Level", "", 0.9f, 0, 1, 0, nullptr},
  {"Snare Tune", "st", 0, -12, 12, 0, nullptr},
  {"Snare Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Hat Tune", "st", 0, -12, 12, 0, nullptr},
  {"Hat Level", "", 0.7f, 0, 1, 0, nullptr},
  {"Clap Tune", "st", 0, -12, 12, 0, nullptr},
  {"Clap Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Tom Tune", "st", 0, -12, 12, 0, nullptr},
  {"Tom Level", "", 0.8f, 0, 1, 0, nullptr},
  {"Accent", "", 0.5f, 0, 1, 0, nullptr},
  {"Cymbal Tune", "st", 0, -12, 12, 0, nullptr},
  {"Cymbal Level", "", 0.6f, 0, 1, 0, nullptr},
  {"Rim Tune", "st", 0, -12, 12, 0, nullptr},
  {"Rim Level", "", 0.7f, 0, 1, 0, nullptr},
  {"Kick Decay", "s", 0.45f, 0.05f, 3, 0, nullptr},
  {"Snare Decay", "s", 0.25f, 0.05f, 1.5f, 0, nullptr},
  {"Hat Decay", "s", 0.35f, 0.05f, 2, 0, nullptr},
  {"Clap Decay", "s", 0.35f, 0.05f, 2, 0, nullptr},
  {"Tom Decay", "s", 0.45f, 0.05f, 2, 0, nullptr},
  {"Cymbal Decay", "s", 1.5f, 0.2f, 5, 0, nullptr},
  {"Rim Decay", "s", 0.05f, 0.01f, 0.5f, 0, nullptr},
  {"Kick Attack", "", 0.5f, 0, 1, 0, nullptr},
  {"Snare Tone", "Hz", 7000, 1000, 12000, 0, nullptr},
  {"Clap Tone", "Hz", 1300, 400, 5000, 0, nullptr},
  {"Tom Tone", "Hz", 5000, 500, 12000, 0, nullptr},
  {"Kick Pan", "", 0, -1, 1, 0, nullptr},
  {"Snare Pan", "", 0.1f, -1, 1, 0, nullptr},
  {"Hat Pan", "", -0.35f, -1, 1, 0, nullptr},
  {"Clap Pan", "", -0.1f, -1, 1, 0, nullptr},
  {"Tom Pan", "", 0.3f, -1, 1, 0, nullptr},
  {"Cymbal Pan", "", 0.45f, -1, 1, 0, nullptr},
  {"Rim Pan", "", 0.2f, -1, 1, 0, nullptr},
  {"Curve", "", 0, -1, 1, 0, nullptr}};

}  // namespace SOUND::PLUGINS::MACHINE
