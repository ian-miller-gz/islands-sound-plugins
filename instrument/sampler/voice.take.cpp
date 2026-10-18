// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../../../inventory.hpp"
#include "voice.internal.hpp"

void SOUND::PLUGINS::SAMPLER::take(Synth &synth) {
  const Inventory &pool = INVENTORY::held();
  const Float ratio = Float(RATE) / Float(synth.rate);
  for (const Stock &stock : pool.stocks) {
    if (stock.kind != KIND::AUDIO || stock.take.lanes.empty()) continue;
    if (stock.take.lanes[0].empty()) continue;
    synth.clips.push_back(
      {.name = stock.name, .lanes = stock.take.lanes, .ratio = ratio});
  }
}

auto SOUND::PLUGINS::SAMPLER::frames(const Clip &clip) -> Whole {
  return clip.lanes.empty() ? 0 : clip.lanes[0].size();
}
