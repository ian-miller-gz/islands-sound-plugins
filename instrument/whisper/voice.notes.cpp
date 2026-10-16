// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Change = void (*)(WHISPER::Mouth &, CORE::VOICE::Note &, Whole);

void kept(WHISPER::Mouth &, CORE::VOICE::Note &, Whole) {}

void struck(WHISPER::Mouth &mouth, CORE::VOICE::Note &note, Whole at) {
  CORE::GLOTTIS::reset(mouth.hisses[at].source);
  WHISPER::shape(mouth, at, true);
  CORE::ENVELOPE::strike(mouth.hisses[at].gate, mouth.envelope, note.velocity);
}

void lifted(WHISPER::Mouth &mouth, CORE::VOICE::Note &, Whole at) {
  CORE::ENVELOPE::lift(mouth.hisses[at].gate);
}

constexpr Change CHANGES[] = {kept, struck, kept, lifted};
static_assert(std::size(CHANGES) == CORE::VOICE::LIFTED + 1);

void voice(WHISPER::Mouth &mouth, CORE::VOICE::Note &note) {
  const Whole at = Whole(&note - mouth.allocator.notes);
  if (at >= WHISPER::HISSES || note.change > CORE::VOICE::LIFTED) return;
  CHANGES[note.change](mouth, note, at);
}

void release(WHISPER::Mouth &mouth) {
  CORE::VOICE::Allocator &allocator = mouth.allocator;
  for (Whole &key : allocator.keys) key = 0;
  for (CORE::VOICE::Note &note : allocator.notes) {
    if (!note.held) continue;
    note.held = false;
    note.change = CORE::VOICE::LIFTED;
  }
  CORE::VOICE::drain(
    allocator, [&mouth](CORE::VOICE::Note &note) { ::voice(mouth, note); });
}

}  // namespace

void SOUND::PLUGINS::WHISPER::apply(
  Mouth &mouth, const AUDIO::PLUGIN::Event &event) {
  const Whole row = CORE::VOICE::apply(
    mouth.allocator, event,
    [&mouth](CORE::VOICE::Note &note) { ::voice(mouth, note); });
  if (row >= PARAMETERS) return;
  const Float before = mouth.rows[row];
  mouth.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  if (row == MODE && mouth.rows[row] != before) ::release(mouth);
  settle(mouth);
  CORE::BLOCK::reset(mouth.stride);
}
