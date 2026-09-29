// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float SILENT = 0.0001f;

auto play(PLUCK::Synth &synth, Whole at) -> Float {
  PLUCK::Voice &voice = synth.voices[at];
  CORE::VOICE::Note &note = synth.allocator.notes[at];
  if (!note.sounding) return 0;
  const Float raw = CORE::NOISE::tick(voice.burst);
  const Float excite = raw - CORE::LINE::tick(voice.pick, raw, voice.notch);
  const Float out = CORE::LINE::read(voice.string, voice.delay, voice.allpass);
  const Float feedback =
    voice.feedbacks[note.held ? PLUCK::RINGING : PLUCK::DAMPED];
  CORE::LINE::write(
    voice.string, excite + feedback * CORE::LINE::damp(voice.loop, out));
  const Float size = CORE::BLOCK::magnitude(out);
  voice.level = size > voice.level ? size : voice.level * synth.fall;
  if (!note.held && voice.level < SILENT) note.sounding = false;
  return out;
}

}  // namespace

auto SOUND::PLUGINS::PLUCK::sound(Synth &synth) -> Float {
  Float sum = 0;
  for (Whole at = 0; at < VOICES; ++at) sum += ::play(synth, at);
  for (CORE::FILTER::Biquad &body : synth.bodies)
    sum = CORE::FILTER::tick(body, sum);
  return CORE::SHAPER::tick(synth.blocker, sum);
}
