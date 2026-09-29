// SPDX-License-Identifier: AGPL-3.0-or-later
#include "percussive.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr PERCUSSIVE::Recipe RECIPES[] = {
  {"Kick", 36, 62.0f, 42.0f, 0.06f, 1.0f, 0, 0.90f, 0.36f},
  {"Snare", 38, 190.0f, 172.0f, 0.02f, 0.45f, 0.30f, 0.75f, 0.17f},
  {"Closed", 42, 0, 0, 0, 0, 0.72f, 0.60f, 0.05f},
  {"Open", 46, 0, 0, 0, 0, 0.72f, 0.55f, 0.38f},
  {"Tom", 45, 116.0f, 84.0f, 0.13f, 1.0f, 0, 0.70f, 0.31f}};
static_assert(sizeof(RECIPES) / sizeof(RECIPES[0]) == PERCUSSIVE::SLOTS);

constexpr STRING::Hot LABELS[] = {"Level", "Decay"};
static_assert(sizeof(LABELS) / sizeof(LABELS[0]) == PERCUSSIVE::LANES);

}  // namespace

auto SOUND::PLUGINS::PERCUSSIVE::slot(Whole index) -> Whole {
  return index < SLOTS * LANES ? index / LANES : SLOTS;
}

auto SOUND::PLUGINS::PERCUSSIVE::lane(Whole index) -> Whole {
  return index % LANES;
}

auto SOUND::PLUGINS::PERCUSSIVE::recipe(Whole slot) -> const Recipe& {
  return RECIPES[slot < SLOTS ? slot : 0];
}

auto SOUND::PLUGINS::PERCUSSIVE::label(Whole index) -> String {
  if (index == GAIN) return "Gain";
  if (index == CHOKE) return "Choke";
  const Whole place = slot(index);
  if (place >= SLOTS) return {};
  return String(RECIPES[place].name) + " " + LABELS[lane(index)];
}
