// SPDX-License-Identifier: AGPL-3.0-or-later
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float FULL = 1;

struct Filtering {
  Flag on;
  Whole kind;
};

constexpr Filtering FILTERINGS[] = {
  {false, CORE::FILTER::LOW},
  {true, CORE::FILTER::LOW},
  {true, CORE::FILTER::BAND},
  {true, CORE::FILTER::HIGH},
  {true, CORE::FILTER::NOTCH}};
static_assert(std::size(FILTERINGS) == std::size(SAMPLER::FILTERS));

auto chosen(const SAMPLER::Synth &synth) -> const Filtering & {
  return FILTERINGS[Whole(synth.rows[SAMPLER::FILTER])];
}

}  // namespace

void SOUND::PLUGINS::SAMPLER::shade(
  Synth &synth, Whole at, Float key, Float contour) {
  const Filtering &filtering = ::chosen(synth);
  if (!filtering.on) return;
  const Float *rows = synth.rows;
  const Float velocity = synth.allocator.notes[at].velocity;
  const Float octaves =
    rows[KEYBOARD] * (key - rows[ROOT]) / CORE::PHASE::OCTAVE +
    rows[ENVELOPE] * contour + rows[WOBBLE] * synth.swing +
    rows[TOUCH] * (velocity - ::FULL);
  const Float cut =
    CORE::BLOCK::scaled(rows[CUTOFF], ::FULL, octaves, LOWEST, BRIGHTEST);
  for (CORE::FILTER::Variable &filter : synth.voices[at].filters)
    CORE::FILTER::settle(
      filter, filtering.kind, cut, rows[RESONANCE], synth.rate);
}

auto SOUND::PLUGINS::SAMPLER::filtered(
  const Synth &synth, Voice &voice, Whole lane, Float in) -> Float {
  if (!::chosen(synth).on) return in;
  return CORE::FILTER::tick(voice.filters[lane], in);
}
