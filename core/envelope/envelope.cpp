// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "envelope.hpp"

namespace {
using namespace SOUND::PLUGINS::CORE;

constexpr Float SINGLE = 1.0f;
constexpr Float RISE = 0.3f;
constexpr Float SETTLE = 0.001f;

auto linear(Float frames, Float from, Float to, Float) -> ENVELOPE::Slope {
  const Float span = frames <= SINGLE ? SINGLE : frames;
  return {SINGLE, (to - from) / span};
}

auto exponential(Float frames, Float from, Float to, Float ratio)
  -> ENVELOPE::Slope {
  if (to == from) return {};
  const Float distance = to > from ? to - from : from - to;
  const Float target = to > from ? to + ratio : to - ratio;
  if (frames <= SINGLE) return {0, target};
  const Float scale = std::exp(-std::log((distance + ratio) / ratio) / frames);
  return {scale, target * (SINGLE - scale)};
}

using Builder = auto (*)(Float, Float, Float, Float) -> ENVELOPE::Slope;

constexpr Builder BUILDERS[ENVELOPE::CURVES] = {linear, exponential};

auto built(const ENVELOPE::Envelope &envelope) -> Builder {
  return BUILDERS
    [envelope.curve < ENVELOPE::CURVES ? envelope.curve : ENVELOPE::LINEAR];
}

}  // namespace

auto SOUND::PLUGINS::CORE::ENVELOPE::frames(Float seconds, Whole rate)
  -> Float {
  return seconds * Float(rate);
}

auto SOUND::PLUGINS::CORE::ENVELOPE::weigh(Float velocity, Float depth)
  -> Float {
  const Float struck = velocity < 0 ? 0 : velocity > FULL ? FULL : velocity;
  return FULL - depth + depth * struck;
}

void SOUND::PLUGINS::CORE::ENVELOPE::shape(Envelope &envelope, Whole rate) {
  const Builder build = ::built(envelope);
  const Float sustain = envelope.sustain;
  envelope.slopes[IDLE] = {0, 0};
  envelope.slopes[RISING] = build(frames(envelope.attack, rate), 0, FULL, RISE);
  envelope.slopes[FALLING] =
    build(frames(envelope.decay, rate), FULL, sustain, SETTLE);
  envelope.slopes[HELD] = {0, sustain};
  envelope.slopes[LEAVING] =
    build(frames(envelope.release, rate), FULL, 0, SETTLE);
  const Float bounds[STAGES] = {0, FULL, sustain, sustain, 0};
  for (Whole stage = 0; stage < STAGES; ++stage)
    envelope.bounds[stage] = bounds[stage];
}

auto SOUND::PLUGINS::CORE::ENVELOPE::ADSR::create(
  Float attack, Float decay, Float sustain, Float release) -> Envelope {
  Envelope envelope;
  envelope.attack = attack;
  envelope.decay = decay;
  envelope.sustain = sustain;
  envelope.release = release;
  return envelope;
}

auto SOUND::PLUGINS::CORE::ENVELOPE::AD::create(Float attack, Float decay)
  -> Envelope {
  Envelope envelope = ADSR::create(attack, decay, 0, decay);
  envelope.sustained = false;
  return envelope;
}

auto SOUND::PLUGINS::CORE::ENVELOPE::AR::create(Float attack, Float release)
  -> Envelope {
  return ADSR::create(attack, 0, FULL, release);
}

auto SOUND::PLUGINS::CORE::ENVELOPE::GATE::create(Float depth) -> Envelope {
  Envelope envelope = ADSR::create(0, 0, FULL, 0);
  envelope.depth = depth;
  return envelope;
}
