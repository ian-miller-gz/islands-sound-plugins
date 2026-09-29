// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "../../../plugin.hpp"

namespace SOUND::CORE::TABLE {

struct Row {
  STRING::Hot name;
  STRING::Hot unit;
  Float resting;
  Float least;
  Float most;
  Whole steps;
  const STRING::Hot *words;
};

struct Sheet {
  const Row *rows;
  Whole count;
};

template <Whole COUNT>
constexpr auto sheet(const Row (&rows)[COUNT]) -> Sheet {
  return {rows, COUNT};
}

constexpr Float COARSE = 100;
constexpr Whole DIGITS = 24;

auto found(const Sheet &sheet, Whole index) -> const Row *;
auto label(const Sheet &sheet, Whole index) -> String;
auto resting(const Sheet &sheet, Whole index) -> Float;
auto clamped(const Sheet &sheet, Whole index, Float value) -> Float;
auto notation(const Sheet &sheet, Whole index, Float value) -> String;
auto control(const Sheet &sheet, Whole index, AUDIO::PLUGIN::Control &out)
  -> Flag;
void rest(const Sheet &sheet, Float *rows);

template <class Instance, const Sheet &SHEET>
struct Surface {
  static auto parameters(void *instance) -> Whole {
    return instance == nullptr ? 0 : SHEET.count;
  }
  static auto name(void *instance, Whole index) -> String {
    return instance == nullptr ? String() : label(SHEET, index);
  }
  static auto held(void *instance, Whole index) -> Float {
    if (instance == nullptr || index >= SHEET.count) return 0;
    return static_cast<const Instance *>(instance)->rows[index];
  }
  static auto reading(void *instance, Whole index) -> String {
    if (instance == nullptr || index >= SHEET.count) return {};
    return notation(SHEET, index, held(instance, index));
  }
  static auto control(void *instance, Whole index, AUDIO::PLUGIN::Control &out)
    -> Flag {
    return instance != nullptr && TABLE::control(SHEET, index, out);
  }
};

}  // namespace SOUND::CORE::TABLE
