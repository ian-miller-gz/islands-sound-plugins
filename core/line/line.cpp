// SPDX-License-Identifier: AGPL-3.0-or-later
#include <algorithm>
#include <bit>
#include <cmath>

#include "line.hpp"

namespace {
using namespace SOUND;

auto bounded(const CORE::LINE::Line &line, Float delay) -> Float {
  const Float far = CORE::LINE::reach(line);
  return delay < CORE::LINE::NEAREST ? CORE::LINE::NEAREST
         : delay > far               ? far
                                     : delay;
}

}  // namespace

void SOUND::CORE::LINE::build(Line &line, Whole frames) {
  const Whole size = std::bit_ceil(frames + GUARD);
  line.ring.assign(size, 0);
  line.mask = size - 1;
  line.head = 0;
}

void SOUND::CORE::LINE::clear(Line &line) {
  std::fill(line.ring.begin(), line.ring.end(), 0.0f);
  line.head = 0;
}

auto SOUND::CORE::LINE::reach(const Line &line) -> Float {
  const Whole size = line.ring.size();
  return size > GUARD ? Float(size - GUARD) : NEAREST;
}

void SOUND::CORE::LINE::write(Line &line, Float in) {
  if (line.ring.empty()) return;
  line.ring[line.head] = in;
  line.head = (line.head + 1) & line.mask;
}

auto SOUND::CORE::LINE::read(const Line &line, Whole delay) -> Float {
  if (line.ring.empty()) return 0;
  return line.ring[(line.head - delay) & line.mask];
}

auto SOUND::CORE::LINE::read(const Line &line, Float delay) -> Float {
  const Float place = ::bounded(line, delay);
  const Float whole = std::floor(place);
  const Float part = place - whole;
  const Float near = read(line, Whole(whole));
  const Float far = read(line, Whole(whole) + 1);
  return near + (far - near) * part;
}

auto SOUND::CORE::LINE::read(const Line &line, Float delay, Allpass &allpass)
  -> Float {
  const Float place = ::bounded(line, delay);
  Float whole = std::floor(place);
  Float part = place - whole;
  if (part < THIN && whole > NEAREST) {
    whole -= UNITY;
    part += UNITY;
  }
  const Float coefficient = (UNITY - part) / (UNITY + part);
  const Float near = read(line, Whole(whole));
  const Float far = read(line, Whole(whole) + 1);
  allpass.held = coefficient * (near - allpass.held) + far;
  return allpass.held;
}

auto SOUND::CORE::LINE::read(const Line &line, const Tap *taps, Whole count)
  -> Float {
  Float sum = 0;
  for (Whole tap = 0; tap < count; ++tap)
    sum += taps[tap].gain * read(line, taps[tap].delay);
  return sum;
}
