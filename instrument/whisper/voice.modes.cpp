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
  {CORE::VOICE::POLY, WHISPER::HISSES, false, CORE::VOICE::ALWAYS, false},
  {CORE::VOICE::MONO, SINGLE, false, CORE::VOICE::ALWAYS, true},
  {CORE::VOICE::MONO, SINGLE, true, CORE::VOICE::LEGATO, true}};
static_assert(std::size(ASSIGNS) == std::size(WHISPER::MODES));

auto assigned(const WHISPER::Mouth &mouth) -> const Assign & {
  const Whole at = Whole(mouth.rows[WHISPER::MODE]);
  return ASSIGNS[at < std::size(ASSIGNS) ? at : 0];
}

}  // namespace

void SOUND::PLUGINS::WHISPER::allocate(Mouth &mouth) {
  const ::Assign &assign = ::assigned(mouth);
  CORE::VOICE::Allocator &allocator = mouth.allocator;
  allocator.mode = assign.mode;
  allocator.count = assign.count;
  allocator.legato = assign.legato;
  allocator.steal = CORE::VOICE::OLDEST;
  allocator.priority = CORE::VOICE::LAST;
  allocator.glide.time = assign.glides ? mouth.rows[GLIDE] : 0;
  allocator.glide.slide = assign.slide;
  CORE::VOICE::settle(allocator, mouth.rate / STRIDE);
}
