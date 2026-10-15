// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float DRIVE = 1;
constexpr Whole FIRST = 0;

using Settle = void (*)(SUPERSAW::Voice &, Whole, Float, Float, Whole);
using Pass = auto (*)(SUPERSAW::Voice &, Float) -> Float;

void ladder(
  SUPERSAW::Voice &voice, Whole, Float cutoff, Float emphasis, Whole rate) {
  CORE::FILTER::settle(voice.ladder, cutoff, emphasis, DRIVE, rate);
}

void variable(
  SUPERSAW::Voice &voice, Whole kind, Float cutoff, Float emphasis,
  Whole rate) {
  CORE::FILTER::settle(voice.variable, kind, cutoff, emphasis, rate);
}

auto ladder(SUPERSAW::Voice &voice, Float in) -> Float {
  return CORE::FILTER::tick(voice.ladder, in);
}

auto variable(SUPERSAW::Voice &voice, Float in) -> Float {
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
static_assert(std::size(MODES) == std::size(SUPERSAW::MODES));

auto chosen(const Float *rows) -> const Mode & {
  const Whole mode = Whole(rows[SUPERSAW::MODE]);
  return MODES[mode < std::size(MODES) ? mode : FIRST];
}

}  // namespace

void SOUND::PLUGINS::SUPERSAW::settle(
  const Synth &synth, Voice &voice, Float cutoff) {
  const ::Mode &mode = ::chosen(synth.rows);
  mode.settle(voice, mode.kind, cutoff, synth.rows[RESONANCE], synth.rate);
}

auto SOUND::PLUGINS::SUPERSAW::filter(
  const Synth &synth, Voice &voice, Float in) -> Float {
  return ::chosen(synth.rows).pass(voice, in);
}
