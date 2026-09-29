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

}  // namespace SOUND::PLUGINS::CORE::BLOCK
