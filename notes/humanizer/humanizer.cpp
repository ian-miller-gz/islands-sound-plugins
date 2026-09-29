// SPDX-License-Identifier: AGPL-3.0-or-later
#include "humanizer.internal.hpp"

namespace {
using namespace SOUND;

using Surface = CORE::TABLE::Surface<HUMANIZER::Humanizer, HUMANIZER::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *humanizer = new HUMANIZER::Humanizer{};
  CORE::TABLE::rest(HUMANIZER::SHEET, humanizer->rows);
  humanizer->out = {humanizer->notes, PLUGIN::ROOM, 0};
  CORE::NOTES::sow(humanizer->white, humanizer->rows[HUMANIZER::SEED]);
  return humanizer;
}

void destroy(void *instance) {
  delete static_cast<HUMANIZER::Humanizer *>(instance);
}

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = HUMANIZER::render,
  .meter = nullptr,
  .destroy = destroy,
  .parameters = Surface::parameters,
  .name = Surface::name,
  .reading = Surface::reading,
  .held = Surface::held,
  .control = Surface::control,
  .ins = {{AUDIO::PLUGIN::Port::NOTES}, {AUDIO::PLUGIN::Port::CONTROL}},
  .outs = {{AUDIO::PLUGIN::Port::NOTES}}};

[[maybe_unused]] const Flag offered = PLUGIN::offer(
  {.name = "humanizer", .surface = &surface, .answer = HUMANIZER::answer});

}  // namespace

auto SOUND::HUMANIZER::answer(
  void *instance, const AUDIO::PLUGIN::Event *, Whole,
  AUDIO::PLUGIN::Event *out, Whole room) -> Whole {
  auto &humanizer = *static_cast<Humanizer *>(instance);
  return CORE::NOTES::drain(humanizer.out, out, room);
}
