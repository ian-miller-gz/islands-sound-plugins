// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float AIR = 4000.0f;
constexpr Float SNAP = 0.002f;
constexpr Float FADE = 0.03f;
constexpr Float SWITCH = 0.005f;
constexpr Float ATTACK = 0.005f;
constexpr Float RELEASE = 0.05f;

void detector(CORE::DYNAMICS::Detector &detector, Whole rate) {
  detector.attack = SNAP;
  detector.release = FADE;
  CORE::DYNAMICS::settle(detector, rate);
}

void hiss(VOCODER::Hiss &hiss, Whole rate) {
  CORE::FILTER::settle(
    hiss.high, CORE::FILTER::HIGH, AIR, CORE::FILTER::FLAT, 0, rate);
  ::detector(hiss.air, rate);
  ::detector(hiss.whole, rate);
  hiss.weight.time = SWITCH;
  CORE::MODULATOR::settle(hiss.weight, rate);
  CORE::NOISE::seed(hiss.white, CORE::NOISE::SEED);
}

}  // namespace

void SOUND::PLUGINS::VOCODER::build(Vocoder &vocoder) {
  vocoder.allocator.count = CARRIERS;
  for (Whole at = 0; at < CARRIERS; ++at)
    CORE::NOISE::seed(
      vocoder.carriers[at].white,
      CORE::NOISE::SEED ^ (CORE::NOISE::Register(at + 1) * SEEDING));
  vocoder.envelope = CORE::ENVELOPE::AR::create(::ATTACK, ::RELEASE);
  vocoder.envelope.trigger = CORE::ENVELOPE::RESUME;
  CORE::ENVELOPE::shape(vocoder.envelope, vocoder.rate);
  ::hiss(vocoder.hiss, vocoder.rate);
}
