// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Colour = auto (*)(WHISPER::Hiss &) -> Float;

constexpr Colour NOISES[] = {
  [](WHISPER::Hiss &hiss) { return CORE::NOISE::tick(hiss.white); },
  [](WHISPER::Hiss &hiss) { return CORE::NOISE::tick(hiss.pink); },
  [](WHISPER::Hiss &hiss) { return CORE::NOISE::tick(hiss.brown); },
  [](WHISPER::Hiss &hiss) { return CORE::NOISE::tick(hiss.blue); }};
static_assert(std::size(NOISES) == WHISPER::COLOURS);

auto excited(const WHISPER::Mouth &mouth, WHISPER::Hiss &hiss) -> Float {
  const Whole colour =
    mouth.colour < WHISPER::COLOURS ? mouth.colour : WHISPER::WHITE;
  const Float noise = NOISES[colour](hiss) * hiss.makeup * mouth.hush;
  const Float pulse = CORE::GLOTTIS::tick(hiss.source, mouth.table);
  return noise + pulse * mouth.voicing;
}

auto hissed(WHISPER::Mouth &mouth, CORE::VOICE::Note &note, WHISPER::Hiss &hiss)
  -> Float {
  const Float level = CORE::ENVELOPE::tick(hiss.gate, mouth.envelope);
  note.level = level;
  if (!CORE::ENVELOPE::sounding(hiss.gate)) note.sounding = false;
  return CORE::FILTER::tick(hiss.formant, ::excited(mouth, hiss)) * level;
}

}  // namespace

auto SOUND::PLUGINS::WHISPER::breathe(Mouth &mouth) -> CORE::STEREO::Pair {
  CORE::STEREO::Pair sum;
  for (Whole at = 0; at < HISSES; ++at) {
    CORE::VOICE::Note &note = mouth.allocator.notes[at];
    if (!note.sounding) continue;
    Hiss &hiss = mouth.hisses[at];
    CORE::STEREO::add(sum, ::hissed(mouth, note, hiss), hiss.pan);
  }
  return sum;
}
