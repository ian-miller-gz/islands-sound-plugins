// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

using Surface = CORE::TABLE::Surface<DRAWBAR::Organ, DRAWBAR::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *organ = new DRAWBAR::Organ{.rate = rate, .channels = channels};
  CORE::TABLE::rest(DRAWBAR::SHEET, organ->rows);
  CORE::OSCILLATOR::build(organ->sine);
  DRAWBAR::build(organ->scanner, rate);
  DRAWBAR::build(organ->rotary, rate);
  DRAWBAR::settle(*organ);
  return organ;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<DRAWBAR::Organ *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<DRAWBAR::Organ *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = DRAWBAR::render,
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
  PLUGIN::offer({.name = "drawbar", .surface = &surface});

}  // namespace
