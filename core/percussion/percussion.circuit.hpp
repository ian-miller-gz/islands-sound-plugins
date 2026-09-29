// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../block/block.hpp"
#include "../envelope/envelope.hpp"
#include "../filter/filter.hpp"
#include "../noise/noise.hpp"
#include "../oscillator/oscillator.hpp"
#include "../shaper/shaper.hpp"

namespace SOUND::CORE::PERCUSSION {

constexpr Whole BANK = 6;
constexpr Float METALLIC[BANK] = {205.3f, 304.4f, 369.6f,
                                  522.7f, 540.0f, 800.0f};

struct Resonator {
  Float turn = 0;
  Float damp = 0;
  Float sine = 0;
  Float cosine = 0;
};

struct Metal {
  OSCILLATOR::Oscillator oscillators[BANK];
  Whole count = BANK;
};

struct Sweep {
  Resonator body;
  ENVELOPE::Envelope drop;
  ENVELOPE::Gate gate;
  Float base = 0;
  Float depth = 0;
};

enum Height : Whole { LOW, MID, HIGH, HEIGHTS };

struct Key {
  Whole pitch;
  Whole drum;
};

auto struck(Float velocity) -> Float;
auto turn(Float hertz, Whole rate) -> Float;
auto fall(Float seconds, Whole rate) -> Float;
void shape(ENVELOPE::Envelope &envelope, Float seconds, Whole rate);

void settle(Resonator &resonator, Float hertz, Float seconds, Whole rate);
void strike(Resonator &resonator, Float level);
auto tick(Resonator &resonator) -> Float;

void settle(
  Metal &metal, const Float *hertz, Whole count, Float ratio, Whole rate);
auto tick(Metal &metal) -> Float;

void settle(Sweep &sweep, Float decay, Float seconds, Float depth, Whole rate);
void strike(Sweep &sweep, Float base, Float level);
auto tick(Sweep &sweep) -> Float;

auto keyed(const Key *keys, Whole count, Whole pitch, Whole fallback) -> Whole;

template <Whole COUNT>
auto keyed(const Key (&keys)[COUNT], Whole pitch, Whole fallback) -> Whole {
  return keyed(keys, COUNT, pitch, fallback);
}

template <class Sound, class Apply>
auto play(
  AUDIO::PLUGIN::Sample *const *lanes, Whole channels, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count, Sound &&sound,
  Apply &&apply) -> Float {
  BLOCK::Cursor cursor;
  Float peak = 0;
  for (Whole frame = 0; frame < frames; ++frame) {
    BLOCK::due(cursor, events, count, frame, apply);
    const Float sample = BLOCK::clipped(sound());
    for (Whole channel = 0; channel < channels; ++channel)
      lanes[channel][frame] = sample;
    const Float size = BLOCK::magnitude(sample);
    if (size > peak) peak = size;
  }
  BLOCK::rest(cursor, events, count, apply);
  return peak;
}

}  // namespace SOUND::CORE::PERCUSSION
