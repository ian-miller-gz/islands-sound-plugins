// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cmath>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float DRIVE = 1;
constexpr Float OCTAVE = 12;
constexpr Float FULL = 1;
constexpr Whole FIRST = 0;

using Settle = void (*)(WAVETABLE::Voice &, Whole, Float, Float, Whole);
using Pass = auto (*)(WAVETABLE::Voice &, Float) -> Float;

void ladder(
  WAVETABLE::Voice &voice, Whole, Float cutoff, Float emphasis, Whole rate) {
  CORE::FILTER::settle(voice.ladder, cutoff, emphasis, DRIVE, rate);
}

void variable(
  WAVETABLE::Voice &voice, Whole kind, Float cutoff, Float emphasis,
  Whole rate) {
  CORE::FILTER::settle(voice.variable, kind, cutoff, emphasis, rate);
}

auto ladder(WAVETABLE::Voice &voice, Float in) -> Float {
  return CORE::FILTER::tick(voice.ladder, in);
}

auto variable(WAVETABLE::Voice &voice, Float in) -> Float {
  return CORE::FILTER::tick(voice.variable, in);
}

struct Mode {
  Settle settle;
  Pass pass;
  Whole kind;
};

constexpr Mode MODES[] = {
  {ladder, ladder, CORE::FILTER::LOW},
  {variable, variable, CORE::FILTER::LOW},
  {variable, variable, CORE::FILTER::BAND},
  {variable, variable, CORE::FILTER::HIGH}};
static_assert(std::size(MODES) == std::size(WAVETABLE::MODES));

auto chosen(const Float *rows) -> const Mode & {
  const Whole mode = Whole(rows[WAVETABLE::MODE]);
  return MODES[mode < std::size(MODES) ? mode : FIRST];
}

}  // namespace

void SOUND::PLUGINS::WAVETABLE::settle(
  const Synth &synth, Voice &voice, const CORE::VOICE::Note &note, Float key,
  Float contour) {
  const Float *rows = synth.rows;
  Float octaves = rows[KEYBOARD] * (key - REFERENCE) / ::OCTAVE;
  octaves += rows[ENVELOPE] * contour;
  octaves += rows[TOUCH] * (note.velocity - ::FULL);
  const ::Mode &mode = ::chosen(rows);
  mode.settle(
    voice, mode.kind, rows[CUTOFF] * std::exp2(octaves), rows[RESONANCE],
    synth.rate);
}

auto SOUND::PLUGINS::WAVETABLE::filter(
  const Synth &synth, Voice &voice, Float in) -> Float {
  return ::chosen(synth.rows).pass(voice, in);
}
