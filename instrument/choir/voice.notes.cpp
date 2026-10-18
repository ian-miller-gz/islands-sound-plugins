// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Change = void (*)(CHOIR::Choir &, CORE::VOICE::Note &, CHOIR::Part &);

void kept(CHOIR::Choir &, CORE::VOICE::Note &, CHOIR::Part &) {}

void struck(CHOIR::Choir &choir, CORE::VOICE::Note &note, CHOIR::Part &part) {
  for (Whole at = 0; at < CHOIR::SEATS; ++at) {
    CHOIR::Singer &singer = part.singers[at];
    const Float onset = CORE::MODULATOR::draw(choir.seed);
    singer.wait = Whole(CORE::BLOCK::magnitude(onset) * choir.scatter);
    singer.pending = at < choir.singers;
    CORE::MODULATOR::reset(singer.vibrato);
    if (!singer.pending) CORE::ENVELOPE::choke(singer.gate);
  }
  CHOIR::tune(choir, note, part, true);
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

void release(CHOIR::Choir &choir) {
  CORE::VOICE::Allocator &allocator = choir.allocator;
  for (Whole &key : allocator.keys) key = 0;
  for (CORE::VOICE::Note &note : allocator.notes) {
    if (!note.held) continue;
    note.held = false;
    note.change = CORE::VOICE::LIFTED;
  }
  CORE::VOICE::drain(
    allocator, [&choir](CORE::VOICE::Note &note) { ::voice(choir, note); });
}

}  // namespace

void SOUND::PLUGINS::CHOIR::apply(
  Choir &choir, const AUDIO::PLUGIN::Event &event) {
  const Whole row = CORE::VOICE::apply(
    choir.allocator, event,
    [&choir](CORE::VOICE::Note &note) { ::voice(choir, note); });
  if (row >= PARAMETERS) return;
  const Float before = choir.rows[row];
  choir.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  if (row == MODE && choir.rows[row] != before) ::release(choir);
  settle(choir);
  CORE::BLOCK::reset(choir.stride);
}
