// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cstdio>

#include "percussive.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float SHORTEST = 0.01f;
constexpr Float LONGEST = 2.0f;

constexpr STRING::Hot SWITCHES[] = {"Off", "On"};

struct Row {
  STRING::Hot unit;
  Float resting;
  Float least;
  Float most;
  Whole steps;
};

constexpr Row LOUDNESS = {"", 0.80f, 0, 1, 0};
constexpr Row CUTTING = {"", 1, 0, 1, 1};

auto row(Whole index, Row &out) -> Flag {
  if (index == PERCUSSIVE::GAIN || index == PERCUSSIVE::CHOKE) {
    out = index == PERCUSSIVE::GAIN ? LOUDNESS : CUTTING;
    return true;
  }
  const Whole slot = PERCUSSIVE::slot(index);
  if (slot >= PERCUSSIVE::SLOTS) return false;
  const PERCUSSIVE::Recipe &made = PERCUSSIVE::recipe(slot);
  out = PERCUSSIVE::lane(index) == PERCUSSIVE::LEVEL
          ? Row{"", made.level, 0, 1, 0}
          : Row{"s", made.decay, SHORTEST, LONGEST, 0};
  return true;
}

auto snapped(const Row &found, Float value) -> Float {
  const Float span = (found.most - found.least) / Float(found.steps);
  const Float places = (value - found.least) / span + 0.5f;
  return found.least + span * Float(Whole(places < 0 ? 0 : places));
}

}  // namespace

auto SOUND::PLUGINS::PERCUSSIVE::control(
  Whole index, AUDIO::PLUGIN::Control &out) -> Flag {
  Row found;
  if (!::row(index, found)) return false;
  Vector<String> positions;
  if (found.steps != 0) positions = {SWITCHES, SWITCHES + found.steps + 1};
  out = {found.unit,    found.steps, positions,
         found.resting, found.least, found.most};
  return true;
}

auto SOUND::PLUGINS::PERCUSSIVE::resting(Whole index) -> Float {
  Row found;
  return ::row(index, found) ? found.resting : 0;
}

auto SOUND::PLUGINS::PERCUSSIVE::clamped(Whole index, Float value) -> Float {
  Row found;
  if (!::row(index, found)) return value;
  const Float held = value < found.least  ? found.least
                     : value > found.most ? found.most
                                          : value;
  return found.steps == 0 ? held : ::snapped(found, held);
}

auto SOUND::PLUGINS::PERCUSSIVE::notation(Whole index, Float value) -> String {
  Row found;
  if (!::row(index, found)) return {};
  if (found.steps != 0) return String(SWITCHES[Whole(clamped(index, value))]);
  char buffer[24];
  std::snprintf(buffer, sizeof(buffer), "%.2f%s", value, found.unit);
  return buffer;
}
