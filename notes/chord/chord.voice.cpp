// SPDX-License-Identifier: AGPL-3.0-or-later
#include "chord.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto chosen(const CHORD::Chord &chord, Whole index, Whole count) -> Whole {
  const Whole place = Whole(chord.rows[index]);
  return place < count ? place : 0;
}

auto moved(Whole pitch, Integer octaves) -> Whole {
  if (pitch == CORE::NOTES::SILENT) return pitch;
  const Integer steps = octaves * static_cast<Integer>(CORE::NOTES::OCTAVE);
  return CORE::NOTES::placed(static_cast<Integer>(pitch) + steps);
}

void widen(const CHORD::Chord &chord, Whole count, Whole *pitches) {
  const Whole spread = ::chosen(chord, CHORD::SPREAD, CHORD::SPREADS);
  Whole low = CORE::NOTES::SILENT;
  Whole high = 0;
  for (Whole tone = 0; tone < count; ++tone) {
    if (pitches[tone] == CORE::NOTES::SILENT) continue;
    if (low == CORE::NOTES::SILENT || pitches[tone] < low) low = pitches[tone];
    if (pitches[tone] > high) high = pitches[tone];
  }
  if (low == CORE::NOTES::SILENT) return;
  if ((spread & CHORD::DOWN) != 0) pitches[CHORD::TONES] = ::moved(low, -1);
  if ((spread & CHORD::UP) != 0) pitches[CHORD::TONES + 1] = ::moved(high, 1);
}

}  // namespace

void SOUND::PLUGINS::CHORD::voice(
  const Chord &chord, Whole key, Whole *pitches) {
  for (Whole at = 0; at < VOICES; ++at) pitches[at] = CORE::NOTES::SILENT;
  const Shape &shape = SHAPES[::chosen(chord, KIND, KINDS)];
  const Whole inversion = ::chosen(chord, INVERSION, INVERSIONS);
  for (Whole tone = 0; tone < shape.count; ++tone) {
    const Whole lifts = (inversion + shape.count - 1 - tone) / shape.count;
    pitches[tone] =
      ::moved(key + shape.steps[tone], static_cast<Integer>(lifts));
  }
  ::widen(chord, shape.count, pitches);
}
