// SPDX-License-Identifier: AGPL-3.0-or-later
#include <cstdio>

#include "transpose.hpp"

namespace {

auto sane(void *instance, Whole index) -> Flag {
  return instance != nullptr && index == SOUND::PLUGINS::TRANSPOSE::STEPS;
}

}  // namespace

auto SOUND::PLUGINS::TRANSPOSE::clamped(Float value) -> Float {
  return value < LEAST ? LEAST : value > MOST ? MOST : value;
}

auto SOUND::PLUGINS::TRANSPOSE::create(Whole rate, Whole channels) -> void * {
  if (rate == 0 || channels == 0) return nullptr;
  auto *shift = new Shift{};
  shift->sounding.assign(HIGHEST + 1, SILENT);
  return shift;
}

void SOUND::PLUGINS::TRANSPOSE::destroy(void *instance) {
  delete static_cast<Shift *>(instance);
}

auto SOUND::PLUGINS::TRANSPOSE::SURFACE::parameters(void *instance) -> Whole {
  return instance == nullptr ? 0 : PARAMETERS;
}

auto SOUND::PLUGINS::TRANSPOSE::SURFACE::name(void *instance, Whole index)
  -> String {
  return ::sane(instance, index) ? "Steps" : String();
}

auto SOUND::PLUGINS::TRANSPOSE::SURFACE::held(void *instance, Whole index)
  -> Float {
  if (!::sane(instance, index)) return 0;
  return static_cast<const Shift *>(instance)->steps;
}

auto SOUND::PLUGINS::TRANSPOSE::SURFACE::reading(void *instance, Whole index)
  -> String {
  if (!::sane(instance, index)) return {};
  char buffer[16];
  std::snprintf(
    buffer, sizeof(buffer), "%+.0f",
    static_cast<double>(held(instance, index)));
  return buffer;
}

auto SOUND::PLUGINS::TRANSPOSE::SURFACE::control(
  void *instance, Whole index, AUDIO::PLUGIN::Control &out) -> Flag {
  if (!::sane(instance, index)) return false;
  out = {.resting = RESTING, .least = LEAST, .most = MOST};
  return true;
}
