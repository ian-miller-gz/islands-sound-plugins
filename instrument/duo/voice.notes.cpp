// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

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

constexpr Visit LOWS[] = {kept, struck, tied, lifted};
constexpr Visit HIGHS[] = {kept, tied, tied, kept};
static_assert(std::size(LOWS) == CORE::VOICE::LIFTED + 1);
static_assert(std::size(HIGHS) == CORE::VOICE::LIFTED + 1);

template <Whole COUNT>
void visit(
  const Visit (&visits)[COUNT], DUO::Synth &synth,
  const CORE::VOICE::Note &note) {
  if (note.change < COUNT) visits[note.change](synth, note);
}

}  // namespace

void SOUND::DUO::strike(Synth &synth, Float velocity) {
  for (Contour *contour : {&synth.adsr, &synth.ar})
    CORE::ENVELOPE::strike(contour->gate, contour->envelope, velocity);
}

void SOUND::DUO::sample(Synth &synth) {
  if (synth.rows[CLOCK] > 0) synth.held = synth.input;
}

void SOUND::DUO::apply(Synth &synth, const AUDIO::PLUGIN::Event &event) {
  CORE::VOICE::apply(
    synth.high, event,
    [&synth](const CORE::VOICE::Note &note) { ::visit(HIGHS, synth, note); });
  const Whole row = CORE::VOICE::apply(
    synth.low, event,
    [&synth](const CORE::VOICE::Note &note) { ::visit(LOWS, synth, note); });
  if (row >= PARAMETERS) return;
  synth.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(synth);
}
