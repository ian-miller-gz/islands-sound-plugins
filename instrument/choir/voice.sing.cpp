// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void wait(CHOIR::Choir &choir, CHOIR::Singer &singer, Float velocity) {
  if (singer.wait > 0) {
    --singer.wait;
    return;
  }
  singer.pending = false;
  CORE::ENVELOPE::strike(singer.gate, choir.envelope, velocity);
}

auto sung(CHOIR::Choir &choir, CHOIR::Singer &singer, Float velocity) -> Float {
  if (singer.pending) ::wait(choir, singer, velocity);
  if (!CORE::ENVELOPE::sounding(singer.gate)) return 0;
  const Float level = CORE::ENVELOPE::tick(singer.gate, choir.envelope);
  const Float pulse = CORE::GLOTTIS::tick(singer.source, choir.table);
  return CORE::FILTER::tick(singer.formant, pulse) * level;
}

auto part(CHOIR::Choir &choir, CORE::VOICE::Note &note, CHOIR::Part &part)
  -> CHOIR::Stereo {
  CHOIR::Stereo sum;
  Float loudest = 0;
  Flag alive = false;
  for (CHOIR::Singer &singer : part.singers) {
    const Float voice = ::sung(choir, singer, note.velocity);
    sum.left += voice * singer.pan.left;
    sum.right += voice * singer.pan.right;
    alive = alive || singer.pending || CORE::ENVELOPE::sounding(singer.gate);
    if (singer.gate.level > loudest) loudest = singer.gate.level;
  }
  note.level = loudest;
  if (!alive) note.sounding = false;
  return sum;
}

}  // namespace

auto SOUND::PLUGINS::CHOIR::sing(Choir &choir) -> Stereo {
  Stereo sum;
  for (Whole at = 0; at < NOTES; ++at) {
    CORE::VOICE::Note &note = choir.allocator.notes[at];
    if (!note.sounding) continue;
    const Stereo voiced = ::part(choir, note, choir.parts[at]);
    sum.left += voiced.left;
    sum.right += voiced.right;
  }
  return {sum.left * BLEND, sum.right * BLEND};
}
