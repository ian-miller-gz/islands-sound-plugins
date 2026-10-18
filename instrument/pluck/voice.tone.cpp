// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float SILENT = 0.0001f;
constexpr Whole LEFT = 0;
constexpr Whole RIGHT = 1;

auto play(PLUCK::Synth &synth, Whole at) -> Float {
  PLUCK::Voice &voice = synth.voices[at];
  CORE::VOICE::Note &note = synth.allocator.notes[at];
  if (!note.sounding) return 0;
  const Float raw = PLUCK::excite(synth, voice);
  const Float excite = raw - CORE::LINE::tick(voice.pick, raw, voice.notch);
  const Float out = CORE::LINE::read(voice.string, voice.delay, voice.allpass);
  const Float feedback =
    voice.feedbacks[note.held ? PLUCK::RINGING : PLUCK::DAMPED];
  const Float damped = CORE::LINE::damp(voice.loop, out);
  CORE::LINE::write(
    voice.string, excite + feedback * PLUCK::disperse(voice, damped));
  const Float size = CORE::BLOCK::magnitude(out);
  voice.level = size > voice.level ? size : voice.level * synth.fall;
  if (!note.held && voice.level < SILENT) note.sounding = false;
  return out;
}

auto shape(PLUCK::Synth &synth, Whole side, Float in) -> Float {
  Float out = in;
  for (CORE::FILTER::Biquad &body : synth.bodies[side])
    out = CORE::FILTER::tick(body, out);
  return CORE::SHAPER::tick(synth.blockers[side], out);
}

}  // namespace

auto SOUND::PLUGINS::PLUCK::sound(Synth &synth) -> CORE::STEREO::Pair {
  CORE::STEREO::Pair sum;
  for (Whole at = 0; at < VOICES; ++at)
    CORE::STEREO::add(sum, ::play(synth, at), synth.voices[at].gains);
  return {::shape(synth, LEFT, sum.left), ::shape(synth, RIGHT, sum.right)};
}
