// SPDX-License-Identifier: AGPL-3.0-or-later
#include <iterator>

#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Whole ONE = 1;
constexpr Whole TWO = ONE << 1;
constexpr Whole THREE = ONE << 2;
constexpr Whole FOUR = ONE << 3;
constexpr Whole FIVE = ONE << 4;
constexpr Whole SIX = ONE << 5;
constexpr Whole NONE = 0;
constexpr Whole FIVES = ONE | TWO | THREE | FOUR | FIVE;
constexpr Whole ALL = FIVES | SIX;

constexpr OPERATOR::Routing ROUTINGS[] = {
  {ONE | THREE, {TWO, NONE, FOUR, FIVE, SIX, NONE}, SIX, SIX},
  {ONE | THREE, {TWO, NONE, FOUR, FIVE, SIX, NONE}, TWO, TWO},
  {ONE | FOUR, {TWO, THREE, NONE, FIVE, SIX, NONE}, SIX, SIX},
  {ONE | FOUR, {TWO, THREE, NONE, FIVE, SIX, NONE}, SIX, FOUR},
  {ONE | THREE | FIVE, {TWO, NONE, FOUR, NONE, SIX, NONE}, SIX, SIX},
  {ONE | THREE | FIVE, {TWO, NONE, FOUR, NONE, SIX, NONE}, SIX, FIVE},
  {ONE | THREE, {TWO, NONE, FOUR | FIVE, NONE, SIX, NONE}, SIX, SIX},
  {ONE | THREE, {TWO, NONE, FOUR | FIVE, NONE, SIX, NONE}, FOUR, FOUR},
  {ONE | THREE, {TWO, NONE, FOUR | FIVE, NONE, SIX, NONE}, TWO, TWO},
  {ONE | FOUR, {TWO, THREE, NONE, FIVE | SIX, NONE, NONE}, THREE, THREE},
  {ONE | FOUR, {TWO, THREE, NONE, FIVE | SIX, NONE, NONE}, SIX, SIX},
  {ONE | THREE, {TWO, NONE, FOUR | FIVE | SIX, NONE, NONE, NONE}, TWO, TWO},
  {ONE | THREE, {TWO, NONE, FOUR | FIVE | SIX, NONE, NONE, NONE}, SIX, SIX},
  {ONE | THREE, {TWO, NONE, FOUR, FIVE | SIX, NONE, NONE}, SIX, SIX},
  {ONE | THREE, {TWO, NONE, FOUR, FIVE | SIX, NONE, NONE}, TWO, TWO},
  {ONE, {TWO | THREE | FIVE, NONE, FOUR, NONE, SIX, NONE}, SIX, SIX},
  {ONE, {TWO | THREE | FIVE, NONE, FOUR, NONE, SIX, NONE}, TWO, TWO},
  {ONE, {TWO | THREE | FOUR, NONE, NONE, FIVE, SIX, NONE}, THREE, THREE},
  {ONE | FOUR | FIVE, {TWO, THREE, NONE, SIX, SIX, NONE}, SIX, SIX},
  {ONE | TWO | FOUR,
   {THREE, THREE, NONE, FIVE | SIX, NONE, NONE},
   THREE,
   THREE},
  {ONE | TWO | FOUR | FIVE, {THREE, THREE, NONE, SIX, SIX, NONE}, THREE, THREE},
  {ONE | THREE | FOUR | FIVE, {TWO, NONE, SIX, SIX, SIX, NONE}, SIX, SIX},
  {ONE | TWO | FOUR | FIVE, {NONE, THREE, NONE, SIX, SIX, NONE}, SIX, SIX},
  {FIVES, {NONE, NONE, SIX, SIX, SIX, NONE}, SIX, SIX},
  {FIVES, {NONE, NONE, NONE, SIX, SIX, NONE}, SIX, SIX},
  {ONE | TWO | FOUR, {NONE, THREE, NONE, FIVE | SIX, NONE, NONE}, SIX, SIX},
  {ONE | TWO | FOUR, {NONE, THREE, NONE, FIVE | SIX, NONE, NONE}, THREE, THREE},
  {ONE | THREE | SIX, {TWO, NONE, FOUR, FIVE, NONE, NONE}, FIVE, FIVE},
  {ONE | TWO | THREE | FIVE, {NONE, NONE, FOUR, NONE, SIX, NONE}, SIX, SIX},
  {ONE | TWO | THREE | SIX, {NONE, NONE, FOUR, FIVE, NONE, NONE}, FIVE, FIVE},
  {FIVES, {NONE, NONE, NONE, NONE, SIX, NONE}, SIX, SIX},
  {ALL, {NONE, NONE, NONE, NONE, NONE, NONE}, SIX, SIX}};
static_assert(std::size(ROUTINGS) == Whole(OPERATOR::ALGORITHMS));

}  // namespace

auto SOUND::OPERATOR::bit(Whole unit) -> Whole { return ::ONE << unit; }

auto SOUND::OPERATOR::routed(Float algorithm) -> const Routing& {
  const Whole at = algorithm < 1 ? 0 : Whole(algorithm) - 1;
  return ::ROUTINGS[at < std::size(::ROUTINGS) ? at : 0];
}
