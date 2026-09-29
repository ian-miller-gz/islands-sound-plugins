// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

using Change = void (*)(WHISPER::Mouth &, CORE::VOICE::Note &, Whole);

void kept(WHISPER::Mouth &, CORE::VOICE::Note &, Whole) {}

void struck(WHISPER::Mouth &mouth, CORE::VOICE::Note &note, Whole at) {
  WHISPER::shape(mouth, at);
  CORE::ENVELOPE::strike(mouth.hisses[at].gate, mouth.envelope, note.velocity);
}

void lifted(WHISPER::Mouth &mouth, CORE::VOICE::Note &, Whole at) {
  CORE::ENVELOPE::lift(mouth.hisses[at].gate);
}

constexpr Change CHANGES[] = {kept, struck, kept, lifted};
static_assert(sizeof(CHANGES) / sizeof(CHANGES[0]) == CORE::VOICE::LIFTED + 1);

using Colour = auto (*)(WHISPER::Hiss &) -> Float;

constexpr Colour NOISES[] = {
  [](WHISPER::Hiss &hiss) { return CORE::NOISE::tick(hiss.white); },
  [](WHISPER::Hiss &hiss) { return CORE::NOISE::tick(hiss.pink); },
  [](WHISPER::Hiss &hiss) { return CORE::NOISE::tick(hiss.brown); },
  [](WHISPER::Hiss &hiss) { return CORE::NOISE::tick(hiss.blue); }};
static_assert(sizeof(NOISES) / sizeof(NOISES[0]) == WHISPER::COLOURS);

void voice(WHISPER::Mouth &mouth, CORE::VOICE::Note &note) {
  const Whole at = Whole(&note - mouth.allocator.notes);
  if (at >= WHISPER::VOICES || note.change > CORE::VOICE::LIFTED) return;
  CHANGES[note.change](mouth, note, at);
}

auto hissed(WHISPER::Mouth &mouth, CORE::VOICE::Note &note, WHISPER::Hiss &hiss)
  -> Float {
  const Float level = CORE::ENVELOPE::tick(hiss.gate, mouth.envelope);
  note.level = level;
  if (!CORE::ENVELOPE::sounding(hiss.gate)) note.sounding = false;
  const Whole colour =
    mouth.colour < WHISPER::COLOURS ? mouth.colour : WHISPER::WHITE;
  const Float noise = NOISES[colour](hiss);
  return CORE::FILTER::tick(hiss.formant, noise) * level;
}

}  // namespace

void SOUND::WHISPER::apply(Mouth &mouth, const AUDIO::PLUGIN::Event &event) {
  const Whole row = CORE::VOICE::apply(
    mouth.allocator, event,
    [&mouth](CORE::VOICE::Note &note) { ::voice(mouth, note); });
  if (row >= PARAMETERS) return;
  mouth.rows[row] = CORE::TABLE::clamped(SHEET, row, event.value);
  settle(mouth);
}

auto SOUND::WHISPER::breathe(Mouth &mouth) -> Float {
  Float sum = 0;
  for (Whole at = 0; at < VOICES; ++at) {
    CORE::VOICE::Note &note = mouth.allocator.notes[at];
    if (note.sounding) sum += ::hissed(mouth, note, mouth.hisses[at]);
  }
  return sum * LOUDNESS;
}
