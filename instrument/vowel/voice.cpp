// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<VOWEL::Singer, VOWEL::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *singer = new VOWEL::Singer{.rate = rate, .channels = channels};
  CORE::GLOTTIS::build(singer->table);
  singer->allocator.count = VOWEL::VOICES;
  for (Whole at = 0; at < VOWEL::VOICES; ++at)
    CORE::GLOTTIS::seed(
      singer->throats[at].source,
      CORE::NOISE::SEED ^ (CORE::NOISE::Register(at + 1) * VOWEL::SCATTER));
  CORE::TABLE::rest(VOWEL::SHEET, singer->rows);
  VOWEL::settle(*singer);
  return singer;
}

auto meter(void *instance) -> Float {
  return CORE::BLOCK::read(static_cast<VOWEL::Singer *>(instance)->meter);
}

void destroy(void *instance) { delete static_cast<VOWEL::Singer *>(instance); }

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = VOWEL::render,
  .meter = meter,
  .destroy = destroy,
  .parameters = Surface::parameters,
  .name = Surface::name,
  .reading = Surface::reading,
  .held = Surface::held,
  .control = Surface::control,
  .ins = {{AUDIO::PLUGIN::Port::NOTES}, {AUDIO::PLUGIN::Port::CONTROL}},
  .outs = {{AUDIO::PLUGIN::Port::AUDIO}}};

[[maybe_unused]] const Flag offered = SOUND::PLUGIN::offer(
  {.name = "vowel",
   .type = "instrument",
   .voicing = SOUND::PLUGIN::POLY,
   .surface = &surface});

}  // namespace
