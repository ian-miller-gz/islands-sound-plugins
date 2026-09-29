// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<STEPS::Sequencer, STEPS::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *sequencer = new STEPS::Sequencer{.rate = rate};
  CORE::TABLE::rest(STEPS::SHEET, sequencer->rows);
  STEPS::settle(*sequencer);
  STEPS::run(*sequencer, 0);
  return sequencer;
}

void destroy(void *instance) {
  delete static_cast<STEPS::Sequencer *>(instance);
}

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = STEPS::render,
  .meter = nullptr,
  .destroy = destroy,
  .parameters = Surface::parameters,
  .name = Surface::name,
  .reading = Surface::reading,
  .held = Surface::held,
  .control = Surface::control,
  .ins = {{AUDIO::PLUGIN::Port::NOTES}, {AUDIO::PLUGIN::Port::CONTROL}},
  .outs = {{AUDIO::PLUGIN::Port::NOTES}}};

[[maybe_unused]] const Flag offered = SOUND::PLUGIN::offer(
  {.name = "steps",
   .type = "control",
   .surface = &surface,
   .answer = STEPS::answer});

}  // namespace
