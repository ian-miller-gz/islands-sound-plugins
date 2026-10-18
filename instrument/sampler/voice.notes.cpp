// SPDX-License-Identifier: AGPL-3.0-or-later
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void kept(SAMPLER::Synth &, Whole) {}

void struck(SAMPLER::Synth &synth, Whole at) {
  SAMPLER::Voice &voice = synth.voices[at];
  CORE::VOICE::Note &note = synth.allocator.notes[at];
  voice.clip = Whole(synth.rows[SAMPLER::STOCK]);
  voice.along = 0;
  voice.turned = false;
  voice.held = true;
  voice.reader =
    SAMPLER::reader(synth, SAMPLER::base(synth, voice, Float(note.pitch)));
  for (CORE::SHAPER::Oversampler &down : voice.downs) down = {};
  for (CORE::FILTER::Variable &filter : voice.filters) filter = {};
  CORE::ENVELOPE::strike(voice.loudness, synth.loudness, note.velocity);
  CORE::ENVELOPE::strike(voice.contour, synth.contour, note.velocity);
  SAMPLER::steer(synth, at);
}

void tied(SAMPLER::Synth &synth, Whole at) {
  SAMPLER::Voice &voice = synth.voices[at];
  const Float velocity = synth.allocator.notes[at].velocity;
  voice.held = true;
  CORE::ENVELOPE::tie(voice.loudness, synth.loudness, velocity);
  CORE::ENVELOPE::tie(voice.contour, synth.contour, velocity);
}

void lifted(SAMPLER::Synth &synth, Whole at) {
  SAMPLER::Voice &voice = synth.voices[at];
  voice.held = false;
  voice.region = SAMPLER::region(synth, synth.clips[voice.clip], false);
  CORE::ENVELOPE::lift(voice.loudness);
  CORE::ENVELOPE::lift(voice.contour);
}

using Visit = void (*)(SAMPLER::Synth &, Whole);

constexpr Visit VISITS[] = {kept, struck, tied, lifted};
static_assert(std::size(VISITS) == CORE::VOICE::LIFTED + 1);

void visit(SAMPLER::Synth &synth, const CORE::VOICE::Note &note) {
  const Whole at = Whole(&note - synth.allocator.notes);
  if (at >= SAMPLER::POLYPHONY || note.change >= std::size(VISITS)) return;
  VISITS[note.change](synth, at);
}

void release(SAMPLER::Synth &synth) {
  CORE::VOICE::Allocator &allocator = synth.allocator;
  for (Whole &key : allocator.keys) key = 0;
  for (Whole at = 0; at < SAMPLER::POLYPHONY; ++at) {
    if (!allocator.notes[at].held) continue;
    allocator.notes[at].held = false;
    lifted(synth, at);
  }
}

}  // namespace

void SOUND::PLUGINS::SAMPLER::land(Synth &synth, Whole row, Float value) {
  if (row >= PARAMETERS) return;
  const Float before = synth.rows[row];
  synth.rows[row] = row == STOCK ? picked(synth, value)
                                 : CORE::TABLE::clamped(SHEET, row, value);
  if (row == MODE && synth.rows[row] != before) ::release(synth);
  settle(synth);
  CORE::BLOCK::reset(synth.stride);
}

void SOUND::PLUGINS::SAMPLER::apply(
  Synth &synth, const AUDIO::PLUGIN::Event &event) {
  if (event.kind == AUDIO::PLUGIN::Event::PROGRAM)
    return land(synth, STOCK, Float(event.index));
  const Whole row = CORE::VOICE::apply(
    synth.allocator, event,
    [&synth](const CORE::VOICE::Note &note) { ::visit(synth, note); });
  land(synth, row, event.value);
}
