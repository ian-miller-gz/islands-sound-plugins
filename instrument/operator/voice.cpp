// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

using Surface = CORE::TABLE::Surface<OPERATOR::Synth, OPERATOR::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *synth = new OPERATOR::Synth{.rate = rate, .channels = channels};
  CORE::TABLE::rest(OPERATOR::SHEET, synth->rows);
  CORE::OSCILLATOR::build(synth->table);
  OPERATOR::settle(*synth);
  return synth;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<OPERATOR::Synth *>(instance)->meter);
}

void destroy(void *instance) {
  delete static_cast<OPERATOR::Synth *>(instance);
}

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = OPERATOR::render,
  .meter = meter,
  .destroy = destroy,
  .parameters = Surface::parameters,
  .name = Surface::name,
  .reading = Surface::reading,
  .held = Surface::held,
  .control = Surface::control,
  .ins = {{AUDIO::PLUGIN::Port::NOTES}, {AUDIO::PLUGIN::Port::CONTROL}},
  .outs = {{AUDIO::PLUGIN::Port::AUDIO}}};

[[maybe_unused]] const Flag offered =
  PLUGIN::offer({.name = "operator", .surface = &surface});

}  // namespace
