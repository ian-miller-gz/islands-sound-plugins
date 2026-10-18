// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Whole SINGLE = 1;

struct Assign {
  Whole mode;
  Whole count;
  Flag legato;
  Whole slide;
  Flag glides;
};

constexpr Assign ASSIGNS[] = {
  {CORE::VOICE::POLY, CHOIR::NOTES, false, CORE::VOICE::ALWAYS, false},
  {CORE::VOICE::MONO, SINGLE, false, CORE::VOICE::ALWAYS, true},
  {CORE::VOICE::MONO, SINGLE, true, CORE::VOICE::LEGATO, true}};
static_assert(std::size(ASSIGNS) == std::size(CHOIR::MODES));

auto assigned(const CHOIR::Choir &choir) -> const Assign & {
  const Whole at = Whole(choir.rows[CHOIR::MODE]);
  return ASSIGNS[at < std::size(ASSIGNS) ? at : 0];
}

}  // namespace

void SOUND::PLUGINS::CHOIR::allocate(Choir &choir) {
  const ::Assign &assign = ::assigned(choir);
  CORE::VOICE::Allocator &allocator = choir.allocator;
  allocator.mode = assign.mode;
  allocator.count = assign.count;
  allocator.legato = assign.legato;
  allocator.steal = CORE::VOICE::OLDEST;
  allocator.priority = CORE::VOICE::LAST;
  allocator.glide.time = assign.glides ? choir.rows[GLIDE] : 0;
  allocator.glide.slide = assign.slide;
  CORE::VOICE::settle(allocator, choir.rate / CONTROL);
}
