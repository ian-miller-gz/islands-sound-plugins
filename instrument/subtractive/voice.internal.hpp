// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include <cstdint>

#include <threads.hpp>

#include "subtractive.hpp"
#include "../../core/oscillator/oscillator.hpp"
#include "../../core/voice/voice.hpp"

namespace SOUND::PLUGINS::SUBTRACTIVE {

enum Stage : Whole { IDLE, RISING, FALLING, HELD, LEAVING };

enum Curve : Whole { LOUDNESS, CONTOUR, CURVES };

struct Envelope {
  Float attack = 0;
  Float decay = 0;
  Float sustain = 0;
  Float release = 0;
  Float rise = 0;
  Float fall = 0;
  Float drop = 0;
};

struct Gate {
  Float level = 0;
  Whole stage = IDLE;
};

struct Voice {
  CORE::OSCILLATOR::Oscillator oscillators[OSCILLATORS];
  Float low = 0;
  Float band = 0;
  Gate gates[CURVES];
};

struct Synth {
  Whole rate = 0;
  Whole channels = 0;
  Float rows[PARAMETERS] = {};
  CORE::VOICE::Allocator allocator = {
    .count = VOICES,
    .mode = CORE::VOICE::POLY,
    .steal = CORE::VOICE::OLDEST,
    .priority = CORE::VOICE::LAST,
    .legato = true};
  Voice voices[VOICES];
  Float turn = 0;
  Whole shape = CORE::OSCILLATOR::SAW;
  Float detune = 1;
  Float damping = 2;
  Envelope envelopes[CURVES];
  Float level[2] = {0, 0};
  THREADS::Shared<Whole> face{0};
};

auto delta(Float seconds, Whole rate) -> Float;

void shape(Envelope &envelope, Whole rate);

void breathe(const Envelope &envelope, Gate &gate);

void settle(Synth &synth);
auto mode(const Synth &synth) -> Whole;

void apply(Synth &synth, const AUDIO::PLUGIN::Event &event);

auto sound(Synth &synth) -> Float;

void render(
  void *instance, AUDIO::PLUGIN::Sample *const *outputs, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count);

}  // namespace SOUND::PLUGINS::SUBTRACTIVE
