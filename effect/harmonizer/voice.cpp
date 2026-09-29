// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<HARMONIZER::Harmonizer, HARMONIZER::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *harmonizer =
    new HARMONIZER::Harmonizer{.rate = rate, .channels = channels};
  HARMONIZER::build(*harmonizer);
  CORE::TABLE::rest(HARMONIZER::SHEET, harmonizer->rows);
  HARMONIZER::settle(*harmonizer);
  return harmonizer;
}

auto meter(void *instance) -> Float {
  auto *harmonizer = static_cast<HARMONIZER::Harmonizer *>(instance);
  return CORE::BLOCK::read(harmonizer->meter);
}

void destroy(void *instance) {
  delete static_cast<HARMONIZER::Harmonizer *>(instance);
}

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = HARMONIZER::render,
  .meter = meter,
  .destroy = destroy,
  .parameters = Surface::parameters,
  .name = Surface::name,
  .reading = Surface::reading,
  .held = Surface::held,
  .control = Surface::control,
  .ins =
    {{AUDIO::PLUGIN::Port::AUDIO},
     {AUDIO::PLUGIN::Port::NOTES},
     {AUDIO::PLUGIN::Port::CONTROL}},
  .outs = {{AUDIO::PLUGIN::Port::AUDIO}}};

[[maybe_unused]] const Flag offered = SOUND::PLUGIN::offer(
  {.name = "harmonizer", .type = "effect", .surface = &surface});

}  // namespace
