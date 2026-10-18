// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;
using RHYTHM::TRIPLETS;

constexpr Whole COMMON = 4;

constexpr RHYTHM::Pattern TABLE[] = {
  {COMMON,
   TRIPLETS,
   {0b100000100000, 0, 0b000100000100, 0b100101100101, 0, 0, 0, 0},
   0b000100000100},
  {COMMON,
   TRIPLETS,
   {0b100000100000, 0b000100000100, 0b101101101101, 0, 0, 0, 0, 0},
   0b000100000100},
  {COMMON,
   TRIPLETS,
   {0b100000100100, 0b000100000100, 0b111111111111, 0, 0, 0, 0, 0},
   0b100100100100}};

constexpr Whole COUNT = sizeof(TABLE) / sizeof(TABLE[0]);

}  // namespace

auto SOUND::PLUGINS::RHYTHM::TRIPLE::pattern(Whole at) -> const Pattern& {
  return ::TABLE[at < ::COUNT ? at : 0];
}
