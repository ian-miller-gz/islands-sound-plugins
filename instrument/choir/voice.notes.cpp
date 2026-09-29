// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

using Change = void (*)(CHOIR::Choir &, CORE::VOICE::Note &, CHOIR::Part &);

void kept(CHOIR::Choir &, CORE::VOICE::Note &, CHOIR::Part &) {}

void struck(CHOIR::Choir &choir, CORE::VOICE::Note &note, CHOIR::Part &part) {
  for (CHOIR::Singer &singer : part.singers) {
    const Float onset = CORE::MODULATOR::draw(choir.seed);
    singer.wait = Whole(CORE::BLOCK::magnitude(onset) * choir.scatter);
    singer.pending = true;
    CORE::MODULATOR::reset(singer.vibrato);
  }
  CHOIR::tune(choir, note, part);
}

void lifted(CHOIR::Choir &, CORE::VOICE::Note &, CHOIR::Part &part) {
  for (CHOIR::Singer &singer : part.singers) {
    singer.pending = false;
    CORE::ENVELOPE::lift(singer.gate);
  }
}

constexpr Change CHANGES[] = {kept, struck, kept, lifted};
static_assert(sizeof(CHANGES) / sizeof(CHANGES[0]) == CORE::VOICE::LIFTED + 1);

void voice(CHOIR::Choir &choir, CORE::VOICE::Note &note) {
  const Whole at = Whole(&note - choir.allocator.notes);
  if (at >= CHOIR::NOTES || note.change > CORE::VOICE::LIFTED) return;
  CHANGES[note.change](choir, note, choir.parts[at]);
}

}  // namespace

void SOUND::CHOIR::apply(Choir &choir, const AUDIO::PLUGIN::Event &event) {
  const Whole row = CORE::VOICE::apply(
    choir.allocator, event,
    [&choir](CORE::VOICE::Note &note) { ::voice(choir, note); });
  if (row >= PARAMETERS) return;
  choir.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(choir);
}
