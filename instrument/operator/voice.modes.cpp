// SPDX-License-Identifier: AGPL-3.0-or-later
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole SINGLE = 1;

struct Assign {
  Whole mode;
  Whole count;
  Flag legato;
  Whole slide;
};

constexpr Assign ASSIGNS[] = {
  {CORE::VOICE::POLY, OPERATOR::VOICES, false, CORE::VOICE::ALWAYS},
  {CORE::VOICE::MONO, SINGLE, false, CORE::VOICE::ALWAYS},
  {CORE::VOICE::MONO, SINGLE, true, CORE::VOICE::LEGATO}};
static_assert(std::size(ASSIGNS) == std::size(OPERATOR::MODES));

auto assigned(const OPERATOR::Synth &synth) -> const Assign & {
  const Whole at = Whole(synth.rows[OPERATOR::MODE]);
  return ASSIGNS[at < std::size(ASSIGNS) ? at : 0];
}

}  // namespace

auto SOUND::PLUGINS::OPERATOR::mode(const Synth &synth) -> Whole {
  return ::assigned(synth).mode;
}

void SOUND::PLUGINS::OPERATOR::allocate(Synth &synth) {
  const ::Assign &assign = ::assigned(synth);
  CORE::VOICE::Allocator &allocator = synth.allocator;
  allocator.mode = assign.mode;
  allocator.count = assign.count;
  allocator.legato = assign.legato;
  allocator.priority = CORE::VOICE::LAST;
  allocator.glide.time = synth.rows[GLIDE];
  allocator.glide.slide = assign.slide;
  CORE::VOICE::settle(allocator, synth.rate);
}
