// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto sing(const ADDITIVE::Synth &synth, ADDITIVE::Bank &bank) -> Float {
  Float sum = 0;
  for (Whole at = 0; at < ADDITIVE::BANK; ++at) {
    bank.gains[at] += bank.slopes[at];
    if (bank.gains[at] == 0) continue;
    sum +=
      bank.gains[at] * CORE::OSCILLATOR::sine(synth.table, bank.phases[at]);
    bank.phases[at] += bank.steps[at];
  }
  return sum;
}

auto play(ADDITIVE::Synth &synth, Whole at) -> Float {
  ADDITIVE::Voice &voice = synth.voices[at];
  CORE::VOICE::Note &note = synth.allocator.notes[at];
  if (!note.sounding) return 0;
  voice.sweep = CORE::ENVELOPE::tick(voice.contour, synth.contour);
  const Float loud = CORE::ENVELOPE::tick(voice.door, synth.door);
  voice.key = CORE::VOICE::tick(note, synth.allocator.glide);
  if (CORE::BLOCK::due(voice.stride, ADDITIVE::STRIDE))
    ADDITIVE::shape(synth, voice, note);
  const Float out = ::sing(synth, voice.bank) * loud;
  note.level = loud;
  if (!CORE::ENVELOPE::sounding(voice.door)) note.sounding = false;
  return out;
}

}  // namespace

auto SOUND::PLUGINS::ADDITIVE::sound(Synth &synth) -> Float {
  synth.swing = CORE::MODULATOR::tick(synth.lfo);
  Float sum = 0;
  for (Whole at = 0; at < VOICES; ++at) sum += ::play(synth, at);
  return sum;
}
