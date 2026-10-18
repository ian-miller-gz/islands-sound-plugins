// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Change = void (*)(VOWEL::Singer &, Whole);

void kept(VOWEL::Singer &, Whole) {}

void struck(VOWEL::Singer &singer, Whole at) {
  VOWEL::Throat &throat = singer.throats[at];
  const Float velocity = singer.allocator.notes[at].velocity;
  CORE::ENVELOPE::strike(throat.gate, singer.envelope, velocity);
  CORE::MODULATOR::reset(throat.vibrato);
  CORE::GLOTTIS::reset(throat.source);
  VOWEL::steer(singer, at, true);
}

void lifted(VOWEL::Singer &singer, Whole at) {
  CORE::ENVELOPE::lift(singer.throats[at].gate);
}

constexpr Change CHANGES[] = {kept, struck, kept, lifted};
static_assert(sizeof(CHANGES) / sizeof(CHANGES[0]) == CORE::VOICE::LIFTED + 1);

void voice(VOWEL::Singer &singer, CORE::VOICE::Note &note) {
  const Whole at = Whole(&note - singer.allocator.notes);
  if (at >= VOWEL::THROATS || note.change > CORE::VOICE::LIFTED) return;
  CHANGES[note.change](singer, at);
}

auto sung(VOWEL::Singer &singer, CORE::VOICE::Note &note, VOWEL::Throat &throat)
  -> Float {
  const Float level = CORE::ENVELOPE::tick(throat.gate, singer.envelope);
  note.level = level;
  if (!CORE::ENVELOPE::sounding(throat.gate)) note.sounding = false;
  const Float pulse = CORE::GLOTTIS::tick(throat.source, singer.table);
  return CORE::FILTER::tick(throat.formant, pulse) * level;
}

}  // namespace

void SOUND::PLUGINS::VOWEL::apply(
  Singer &singer, const AUDIO::PLUGIN::Event &event) {
  const Whole row = CORE::VOICE::apply(
    singer.allocator, event,
    [&singer](CORE::VOICE::Note &note) { ::voice(singer, note); });
  if (row >= PARAMETERS) return;
  const Float before = singer.rows[row];
  singer.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  if (row == MODE && singer.rows[row] != before) release(singer);
  settle(singer);
}

auto SOUND::PLUGINS::VOWEL::sing(Singer &singer) -> CORE::STEREO::Pair {
  if (CORE::BLOCK::due(singer.stride, STRIDE)) steer(singer);
  CORE::STEREO::Pair sum;
  for (Whole at = 0; at < THROATS; ++at) {
    CORE::VOICE::Note &note = singer.allocator.notes[at];
    Throat &throat = singer.throats[at];
    if (note.sounding)
      CORE::STEREO::add(sum, ::sung(singer, note, throat), throat.pan);
  }
  return sum;
}
