// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole TIMBRES[] = {BULL::WAVEA,   BULL::WAVEB,  BULL::BEAT,
                             BULL::SUB,     BULL::CUTOFF, BULL::EMPHASIS,
                             BULL::CONTOUR, BULL::SWEEP,  BULL::CLOSE,
                             BULL::ATTACK,  BULL::DECAY,  BULL::SUSTAIN};

constexpr Whole SETTINGS = std::size(TIMBRES);

constexpr Float SAW = 0;
constexpr Float SQUARE = 1;
constexpr Float OPEN = 0;
constexpr Float HELD = 1;

constexpr Float PRESETS[BULL::MANUAL][SETTINGS] = {
  {SAW, SAW, 4, 0.3f, 420, 0.25f, 2.5f, 0.002f, 0.35f, 0.003f, 0.9f, OPEN},
  {SQUARE, SAW, 2, 0.2f, 240, 0.1f, 2, 0.08f, 0.6f, 0.06f, 0.4f, HELD},
  {SAW, SAW, 9, 0.6f, 170, 0.35f, 1.5f, 0.01f, 1.2f, 0.01f, 1.6f, HELD}};

}  // namespace

void SOUND::PLUGINS::BULL::compose(Synth &synth) {
  for (Whole index = 0; index < PARAMETERS; ++index)
    synth.chain[index] = synth.rows[index];
  const Whole voice = Whole(synth.rows[VOICE]);
  if (voice >= MANUAL) return;
  for (Whole at = 0; at < ::SETTINGS; ++at)
    synth.chain[::TIMBRES[at]] = ::PRESETS[voice][at];
}
