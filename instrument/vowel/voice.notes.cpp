// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

using Change = void (*)(VOWEL::Singer &, CORE::VOICE::Note &, VOWEL::Throat &);

void kept(VOWEL::Singer &, CORE::VOICE::Note &, VOWEL::Throat &) {}

void struck(
  VOWEL::Singer &singer, CORE::VOICE::Note &note, VOWEL::Throat &throat) {
  CORE::ENVELOPE::strike(throat.gate, singer.envelope, note.velocity);
  CORE::MODULATOR::reset(throat.vibrato);
  CORE::GLOTTIS::reset(throat.source);
}

void lifted(VOWEL::Singer &, CORE::VOICE::Note &, VOWEL::Throat &throat) {
  CORE::ENVELOPE::lift(throat.gate);
}

constexpr Change CHANGES[] = {kept, struck, kept, lifted};
static_assert(sizeof(CHANGES) / sizeof(CHANGES[0]) == CORE::VOICE::LIFTED + 1);

void voice(VOWEL::Singer &singer, CORE::VOICE::Note &note) {
  const Whole at = Whole(&note - singer.allocator.notes);
  if (at >= VOWEL::VOICES || note.change > CORE::VOICE::LIFTED) return;
  CHANGES[note.change](singer, note, singer.throats[at]);
}

auto sung(VOWEL::Singer &singer, CORE::VOICE::Note &note, VOWEL::Throat &throat)
  -> Float {
  const Float level = CORE::ENVELOPE::tick(throat.gate, singer.envelope);
  note.level = level;
  if (!CORE::ENVELOPE::sounding(throat.gate)) note.sounding = false;
  const Float cents = CORE::MODULATOR::tick(throat.vibrato);
  const Float pitch = Float(note.pitch) + cents / VOWEL::CENT;
  CORE::GLOTTIS::tune(throat.source, CORE::PHASE::hertz(pitch), singer.rate);
  const Float pulse = CORE::GLOTTIS::tick(throat.source, singer.table);
  return CORE::FILTER::tick(throat.formant, pulse) * level;
}

}  // namespace

void SOUND::VOWEL::apply(Singer &singer, const AUDIO::PLUGIN::Event &event) {
  const Whole row = CORE::VOICE::apply(
    singer.allocator, event,
    [&singer](CORE::VOICE::Note &note) { ::voice(singer, note); });
  if (row >= PARAMETERS) return;
  singer.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(singer);
}

auto SOUND::VOWEL::sing(Singer &singer) -> Float {
  Float sum = 0;
  for (Whole at = 0; at < VOICES; ++at) {
    CORE::VOICE::Note &note = singer.allocator.notes[at];
    if (note.sounding) sum += ::sung(singer, note, singer.throats[at]);
  }
  return sum;
}
