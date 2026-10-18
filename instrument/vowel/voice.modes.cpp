// SPDX-License-Identifier: AGPL-3.0-or-later
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole FIRST = 0;

struct Voicing {
  Whole mode;
  Flag legato;
  Whole slide;
  Flag glided;
};

constexpr Voicing VOICINGS[] = {
  {CORE::VOICE::POLY, false, CORE::VOICE::ALWAYS, false},
  {CORE::VOICE::MONO, false, CORE::VOICE::ALWAYS, true},
  {CORE::VOICE::MONO, true, CORE::VOICE::LEGATO, true}};
static_assert(std::size(VOICINGS) == std::size(VOWEL::MODES));

}  // namespace

void SOUND::PLUGINS::VOWEL::allot(Singer &singer) {
  const Float *rows = singer.rows;
  const Whole at = Whole(rows[MODE]);
  const Voicing &voicing =
    ::VOICINGS[at < std::size(::VOICINGS) ? at : ::FIRST];
  CORE::VOICE::Allocator &allocator = singer.allocator;
  allocator.count = Whole(rows[POLYPHONY]);
  allocator.steal = CORE::VOICE::OLDEST;
  allocator.mode = voicing.mode;
  allocator.legato = voicing.legato;
  allocator.glide.slide = voicing.slide;
  allocator.glide.time = voicing.glided ? rows[GLIDE] : 0;
  CORE::VOICE::settle(allocator, singer.pace);
}

void SOUND::PLUGINS::VOWEL::release(Singer &singer) {
  CORE::VOICE::Allocator &allocator = singer.allocator;
  for (Whole &key : allocator.keys) key = 0;
  for (Whole at = 0; at < THROATS; ++at) {
    CORE::VOICE::Note &note = allocator.notes[at];
    if (!note.held) continue;
    note.held = false;
    CORE::ENVELOPE::lift(singer.throats[at].gate);
  }
}
