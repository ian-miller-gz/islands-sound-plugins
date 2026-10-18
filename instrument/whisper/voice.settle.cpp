// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float FULL = 1.0f;
}  // namespace

void SOUND::PLUGINS::WHISPER::seed(Hiss &hiss, CORE::NOISE::Register state) {
  CORE::NOISE::seed(hiss.white, state);
  CORE::NOISE::seed(hiss.pink, state);
  CORE::NOISE::seed(hiss.brown, state);
  CORE::NOISE::seed(hiss.blue, state);
  CORE::GLOTTIS::seed(hiss.source, state);
}

void SOUND::PLUGINS::WHISPER::settle(Mouth &mouth) {
  const Float *rows = mouth.rows;
  const Whole pace = mouth.rate / STRIDE;
  mouth.gain = rows[GAIN];
  mouth.colour = Whole(rows[COLOUR]);
  mouth.voicing = rows[VOICING];
  mouth.hush = ::FULL - mouth.voicing;
  mouth.envelope = CORE::ENVELOPE::ADSR::create(
    rows[ATTACK], rows[DECAY], rows[SUSTAIN], rows[RELEASE]);
  mouth.envelope.depth = CORE::ENVELOPE::FULL;
  CORE::ENVELOPE::shape(mouth.envelope, mouth.rate);
  allocate(mouth);
  mouth.pole = CORE::MODULATOR::pole(GLIDING, pace);
  mouth.motion.hertz = rows[MOTION];
  CORE::MODULATOR::settle(mouth.motion, pace);
  place(mouth);
  for (Hiss &hiss : mouth.hisses) {
    CORE::GLOTTIS::settle(hiss.source, TENSION, 0, mouth.rate);
    hiss.pan = CORE::STEREO::pan(hiss.place * rows[WIDTH]);
  }
}
