// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <threads.hpp>

#include "../../../plugin.hpp"

namespace SOUND::PLUGINS::CORE::BLOCK {

struct Meter {
  Float level[2] = {0, 0};
  THREADS::Shared<Whole> face{0};
};

void denormals();
void publish(Meter &meter, Float peak);
auto read(const Meter &meter) -> Float;

auto magnitude(Float value) -> Float;
auto clipped(Float value) -> Float;
auto loudest(AUDIO::PLUGIN::Sample *const *lanes, Whole channels, Whole frame)
  -> Float;

struct Cursor {
  Whole next = 0;
};

template <class Apply>
void due(
  Cursor &cursor, const AUDIO::PLUGIN::Event *events, Whole count, Whole frame,
  Apply &&apply) {
  while (cursor.next < count && events[cursor.next].offset <= frame)
    apply(events[cursor.next++]);
}

template <class Apply>
void rest(
  Cursor &cursor, const AUDIO::PLUGIN::Event *events, Whole count,
  Apply &&apply) {
  while (cursor.next < count) apply(events[cursor.next++]);
}

constexpr Float UPPER = 0.1f;
constexpr Float LOWER = 0.05f;

enum Crossing : Whole { STILL, ROSE, FELL, CROSSINGS };

struct Stride {
  Whole left = 0;
};

struct Edge {
  Flag high = false;
  Float last = 0;
  Float since = 0;
};

auto due(Stride &stride, Whole every) -> Flag;
void reset(Stride &stride);

auto lane(
  AUDIO::PLUGIN::Sample *const *lanes, Whole in, Whole channel,
  Whole channels) -> AUDIO::PLUGIN::Sample *;

auto crossed(Edge &edge, Float in) -> Whole;

auto scaled(Float base, Float cv, Float octaves, Float least, Float most)
  -> Float;

}  // namespace SOUND::PLUGINS::CORE::BLOCK
