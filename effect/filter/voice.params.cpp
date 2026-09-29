// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

auto sane(void *instance, Whole index) -> Flag {
  return instance != nullptr && index < FILTER::PARAMETERS;
}

}  // namespace

auto SOUND::PLUGINS::FILTER::SURFACE::parameters(void *instance) -> Whole {
  return instance == nullptr ? 0 : PARAMETERS;
}

auto SOUND::PLUGINS::FILTER::SURFACE::name(void *instance, Whole index)
  -> String {
  return ::sane(instance, index) ? label(index) : String();
}

auto SOUND::PLUGINS::FILTER::SURFACE::held(void *instance, Whole index)
  -> Float {
  if (!::sane(instance, index)) return 0;
  return static_cast<Sieve *>(instance)->rows[index];
}

auto SOUND::PLUGINS::FILTER::SURFACE::reading(void *instance, Whole index)
  -> String {
  if (!::sane(instance, index)) return {};
  return notation(index, held(instance, index));
}

auto SOUND::PLUGINS::FILTER::SURFACE::control(
  void *instance, Whole index, AUDIO::PLUGIN::Control &out) -> Flag {
  return ::sane(instance, index) && FILTER::control(index, out);
}
