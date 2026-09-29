// SPDX-License-Identifier: AGPL-3.0-or-later
#include "noise.hpp"

namespace {
using namespace SOUND::CORE;

constexpr Whole LEFT = 13;
constexpr Whole RIGHT = 17;
constexpr Whole BACK = 5;

constexpr Float POLES[] = {0.99886f, 0.99332f, 0.96900f,
                           0.86650f, 0.55000f, -0.7616f};
constexpr Float GAINS[] = {0.0555179f, 0.0750759f, 0.1538520f,
                           0.3104856f, 0.5329522f, -0.0168980f};
static_assert(sizeof(POLES) / sizeof(POLES[0]) == NOISE::POLES);
static_assert(sizeof(GAINS) / sizeof(GAINS[0]) == NOISE::POLES);
constexpr Float DIRECT = 0.5362f;
constexpr Float DELAYED = 0.115926f;
constexpr Float PINK = 0.11f;

constexpr Float STEP = 0.02f;
constexpr Float LEAK = 1.02f;
constexpr Float BROWN = 3.5f;

constexpr Float BLUE = 1.7f;

}  // namespace

void SOUND::CORE::NOISE::seed(White &white, Register state) {
  white.state = state == 0 ? SEED : state;
}

void SOUND::CORE::NOISE::seed(Pink &pink, Register state) {
  seed(pink.white, state);
}

void SOUND::CORE::NOISE::seed(Brown &brown, Register state) {
  seed(brown.white, state);
}

void SOUND::CORE::NOISE::seed(Blue &blue, Register state) {
  seed(blue.pink, state);
}

void SOUND::CORE::NOISE::seed(Burst &burst, Register state) {
  seed(burst.white, state);
}

auto SOUND::CORE::NOISE::tick(White &white) -> Float {
  Register state = white.state;
  state ^= state << ::LEFT;
  state ^= state >> ::RIGHT;
  state ^= state << ::BACK;
  white.state = state;
  return Float(static_cast<std::int32_t>(state)) / PHASE::HALF;
}

auto SOUND::CORE::NOISE::tick(Pink &pink) -> Float {
  const Float white = tick(pink.white);
  Float sum = pink.last + white * ::DIRECT;
  for (Whole pole = 0; pole < POLES; ++pole) {
    pink.poles[pole] = ::POLES[pole] * pink.poles[pole] + ::GAINS[pole] * white;
    sum += pink.poles[pole];
  }
  pink.last = white * ::DELAYED;
  return sum * ::PINK;
}

auto SOUND::CORE::NOISE::tick(Brown &brown) -> Float {
  brown.level = (brown.level + ::STEP * tick(brown.white)) / ::LEAK;
  return brown.level * ::BROWN;
}

auto SOUND::CORE::NOISE::tick(Blue &blue) -> Float {
  const Float pink = tick(blue.pink);
  const Float value = (pink - blue.last) * ::BLUE;
  blue.last = pink;
  return value;
}
