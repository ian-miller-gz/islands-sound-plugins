// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

void kept(TINE::Synth &, Whole) {}

void wake(TINE::Voice &voice) {
  if (CORE::ENVELOPE::sounding(voice.door)) return;
  CORE::OSCILLATOR::reset(voice.tine, 0);
  CORE::OSCILLATOR::reset(voice.bar, 0);
  CORE::OSCILLATOR::reset(voice.bell, 0);
}

void struck(TINE::Synth &synth, Whole at) {
  TINE::Voice &voice = synth.voices[at];
  const Float velocity = synth.allocator.notes[at].velocity;
  ::wake(voice);
  TINE::shape(synth, at);
  TINE::tune(synth, at);
  CORE::ENVELOPE::strike(voice.door, voice.loudness, velocity);
  CORE::ENVELOPE::strike(voice.strike, synth.strike, velocity);
  CORE::ENVELOPE::strike(voice.ring, synth.ring, velocity);
}

void lifted(TINE::Synth &synth, Whole at) {
  CORE::ENVELOPE::lift(synth.voices[at].door);
}

using Visit = void (*)(TINE::Synth &, Whole);

constexpr Visit VISITS[] = {kept, struck, struck, lifted};
static_assert(std::size(VISITS) == CORE::VOICE::LIFTED + 1);

void visit(TINE::Synth &synth, const CORE::VOICE::Note &note) {
  const Whole at = Whole(&note - synth.allocator.notes);
  if (at >= TINE::VOICES || note.change >= std::size(VISITS)) return;
  VISITS[note.change](synth, at);
}

}  // namespace

void SOUND::TINE::apply(Synth &synth, const AUDIO::PLUGIN::Event &event) {
  const Whole row = CORE::VOICE::apply(
    synth.allocator, event,
    [&synth](const CORE::VOICE::Note &note) { ::visit(synth, note); });
  if (row >= PARAMETERS) return;
  synth.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(synth);
}
