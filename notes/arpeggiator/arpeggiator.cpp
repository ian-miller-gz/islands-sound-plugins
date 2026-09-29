// SPDX-License-Identifier: AGPL-3.0-or-later
#include "arpeggiator.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

using Surface = CORE::TABLE::Surface<ARPEGGIATOR::Arpeggio, ARPEGGIATOR::SHEET>;

auto create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *arpeggio = new ARPEGGIATOR::Arpeggio{.rate = rate};
  CORE::TABLE::rest(ARPEGGIATOR::SHEET, arpeggio->rows);
  arpeggio->out = {arpeggio->notes, SOUND::PLUGIN::ROOM, 0};
  CORE::NOTES::sow(arpeggio->white, arpeggio->rows[ARPEGGIATOR::SEED]);
  ARPEGGIATOR::settle(*arpeggio);
  return arpeggio;
}

void destroy(void *instance) {
  delete static_cast<ARPEGGIATOR::Arpeggio *>(instance);
}

const AUDIO::PLUGIN::Plug surface = {
  .create = create,
  .render = ARPEGGIATOR::render,
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
  {.name = "arpeggiator",
   .type = "notes",
   .surface = &surface,
   .answer = ARPEGGIATOR::answer});

}  // namespace

auto SOUND::PLUGINS::ARPEGGIATOR::answer(
  void *instance, const AUDIO::PLUGIN::Event *, Whole,
  AUDIO::PLUGIN::Event *out, Whole room) -> Whole {
  auto &arpeggio = *static_cast<Arpeggio *>(instance);
  return CORE::NOTES::drain(arpeggio.out, out, room);
}
