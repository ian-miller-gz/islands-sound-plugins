// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Float HALF = 0.5f;

auto synth(void *instance) -> const SAMPLER::Synth & {
  return *static_cast<const SAMPLER::Synth *>(instance);
}

}  // namespace

auto SOUND::PLUGINS::SAMPLER::picked(const Synth &synth, Float value) -> Float {
  const Float most = Float(synth.clips.size() - 1);
  const Float held = value < 0 ? 0 : value > most ? most : value;
  return Float(Whole(held + ::HALF));
}

auto SOUND::PLUGINS::SAMPLER::reading(void *instance, Whole index) -> String {
  if (instance == nullptr || index != STOCK)
    return Surface::reading(instance, index);
  const Synth &held = ::synth(instance);
  return held.clips[Whole(held.rows[STOCK])].name;
}

auto SOUND::PLUGINS::SAMPLER::control(
  void *instance, Whole index, AUDIO::PLUGIN::Control &out) -> Flag {
  if (!Surface::control(instance, index, out)) return false;
  if (index != STOCK) return true;
  const Synth &held = ::synth(instance);
  out.steps = held.clips.size() - 1;
  out.most = Float(out.steps);
  out.labels.clear();
  for (const Clip &clip : held.clips) out.labels.push_back(clip.name);
  return true;
}
