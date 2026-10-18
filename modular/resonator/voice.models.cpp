// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float ONE = 1.0f;

constexpr Whole KINDS[] = {
  CORE::FILTER::LOW, CORE::FILTER::BAND, CORE::FILTER::HIGH,
  CORE::FILTER::NOTCH};
static_assert(std::size(KINDS) == RESONATOR::MODES);

constexpr Whole SIDES[] = {
  CORE::FILTER::LOW, CORE::FILTER::LOW, CORE::FILTER::HIGH, CORE::FILTER::LOW};
static_assert(std::size(SIDES) == RESONATOR::MODES);

auto gain(Float drive) -> Float { return ONE + RESONATOR::BOOST * drive; }

void variable(RESONATOR::Stage &stage, const RESONATOR::Tone &tone) {
  CORE::FILTER::settle(
    stage.variable, KINDS[tone.kind], tone.cutoff, tone.emphasis, tone.drive,
    tone.rate);
}

auto variable(RESONATOR::Stage &stage, Float in) -> Float {
  return CORE::FILTER::tick(stage.variable, in);
}

void ladder(RESONATOR::Stage &stage, const RESONATOR::Tone &tone) {
  CORE::FILTER::settle(
    stage.ladder, tone.cutoff, tone.emphasis, ::gain(tone.drive), tone.rate);
}

auto ladder(RESONATOR::Stage &stage, Float in) -> Float {
  return CORE::FILTER::tick(stage.ladder, in);
}

void diode(RESONATOR::Stage &stage, const RESONATOR::Tone &tone) {
  CORE::FILTER::settle(
    stage.diode, tone.cutoff, tone.emphasis, ::gain(tone.drive), tone.rate);
}

auto diode(RESONATOR::Stage &stage, Float in) -> Float {
  return CORE::FILTER::tick(stage.diode, in);
}

void sallen(RESONATOR::Stage &stage, const RESONATOR::Tone &tone) {
  CORE::FILTER::settle(
    stage.sallen, SIDES[tone.kind], tone.cutoff, tone.emphasis,
    ::gain(tone.drive), tone.rate);
}

auto sallen(RESONATOR::Stage &stage, Float in) -> Float {
  return CORE::FILTER::tick(stage.sallen, in);
}

constexpr RESONATOR::Model MODELS[] = {
  {variable, variable}, {ladder, ladder}, {diode, diode}, {sallen, sallen}};
static_assert(std::size(MODELS) == RESONATOR::MODELS);

}  // namespace

auto SOUND::PLUGINS::RESONATOR::chosen(const Module &module) -> Whole {
  const Whole model = Whole(module.rows[MODEL]);
  return model < MODELS ? model : VARIABLE;
}

auto SOUND::PLUGINS::RESONATOR::model(const Module &module) -> const Model & {
  return ::MODELS[module.model];
}
