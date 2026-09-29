// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"

namespace SOUND::PLUGINS::CORE::ENVELOPE {

enum Stage : Whole { IDLE, RISING, FALLING, HELD, LEAVING, STAGES };

enum Curve : Whole { LINEAR, EXPONENTIAL, CURVES };

enum Trigger : Whole { RESET, RESUME, TRIGGERS };

constexpr Float FULL = 1.0f;

struct Slope {
  Float scale = 1;
  Float step = 0;
};

struct Envelope {
  Float attack = 0;
  Float decay = 0;
  Float sustain = FULL;
  Float release = 0;
  Whole curve = LINEAR;
  Whole trigger = RESET;
  Flag sustained = true;
  Float depth = 0;
  Slope slopes[STAGES];
  Float bounds[STAGES] = {};
};

struct Gate {
  Float level = 0;
  Whole stage = IDLE;
  Float scale = FULL;
};

auto frames(Float seconds, Whole rate) -> Float;
auto weigh(Float velocity, Float depth) -> Float;

void shape(Envelope &envelope, Whole rate);
void strike(Gate &gate, const Envelope &envelope, Float velocity);
void tie(Gate &gate, const Envelope &envelope, Float velocity);
void lift(Gate &gate);
void choke(Gate &gate);
auto sounding(const Gate &gate) -> Flag;
auto tick(Gate &gate, const Envelope &envelope) -> Float;

}  // namespace SOUND::PLUGINS::CORE::ENVELOPE

namespace SOUND::PLUGINS::CORE::ENVELOPE::ADSR {
auto create(Float attack, Float decay, Float sustain, Float release)
  -> Envelope;
}  // namespace SOUND::PLUGINS::CORE::ENVELOPE::ADSR

namespace SOUND::PLUGINS::CORE::ENVELOPE::AD {
auto create(Float attack, Float decay) -> Envelope;
}  // namespace SOUND::PLUGINS::CORE::ENVELOPE::AD

namespace SOUND::PLUGINS::CORE::ENVELOPE::AR {
auto create(Float attack, Float release) -> Envelope;
}  // namespace SOUND::PLUGINS::CORE::ENVELOPE::AR

namespace SOUND::PLUGINS::CORE::ENVELOPE::GATE {
auto create(Float depth) -> Envelope;
}  // namespace SOUND::PLUGINS::CORE::ENVELOPE::GATE

#include "envelope.segments.hpp"
