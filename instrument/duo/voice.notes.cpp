// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void kept(DUO::Synth &, const CORE::VOICE::Note &) {}

void struck(DUO::Synth &synth, const CORE::VOICE::Note &note) {
  DUO::strike(synth, note.velocity);
  DUO::sample(synth);
}

void tied(DUO::Synth &synth, const CORE::VOICE::Note &) { DUO::sample(synth); }

void lifted(DUO::Synth &synth, const CORE::VOICE::Note &) {
  CORE::ENVELOPE::lift(synth.adsr.gate);
  CORE::ENVELOPE::lift(synth.ar.gate);
}

using Visit = void (*)(DUO::Synth &, const CORE::VOICE::Note &);

constexpr Whole CHANGES = CORE::VOICE::LIFTED + 1;

constexpr Visit VISITS[CORE::VOICE::PARTS][CHANGES] = {
  {kept, struck, tied, lifted}, {kept, tied, tied, kept}};

void visit(DUO::Synth &synth, const CORE::VOICE::Note &note) {
  const Whole part = Whole(&note - synth.allocator.notes);
  if (part >= CORE::VOICE::PARTS || note.change >= CHANGES) return;
  VISITS[part][note.change](synth, note);
}

}  // namespace

void SOUND::PLUGINS::DUO::strike(Synth &synth, Float velocity) {
  for (Contour *contour : {&synth.adsr, &synth.ar})
    CORE::ENVELOPE::strike(contour->gate, contour->envelope, velocity);
}

void SOUND::PLUGINS::DUO::sample(Synth &synth) {
  if (synth.rows[CLOCK] > 0) synth.held = synth.input;
}

void SOUND::PLUGINS::DUO::apply(
  Synth &synth, const AUDIO::PLUGIN::Event &event) {
  const Whole row = CORE::VOICE::apply(
    synth.allocator, event,
    [&synth](const CORE::VOICE::Note &note) { ::visit(synth, note); });
  if (row >= PARAMETERS) return;
  synth.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(synth);
}
