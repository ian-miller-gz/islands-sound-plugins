// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

void stride(SAMPLER::Synth &synth) {
  synth.swing = CORE::MODULATOR::tick(synth.lfo);
  for (Whole at = 0; at < SAMPLER::POLYPHONY; ++at)
    if (synth.allocator.notes[at].sounding) SAMPLER::steer(synth, at);
}

void end(SAMPLER::Synth &synth, Whole at) {
  CORE::ENVELOPE::choke(synth.voices[at].loudness);
  CORE::ENVELOPE::choke(synth.voices[at].contour);
  synth.allocator.notes[at].sounding = false;
  synth.allocator.notes[at].level = 0;
}

void sound(
  SAMPLER::Synth &synth, Whole at, AUDIO::PLUGIN::Sample *const *lanes,
  Whole frame) {
  SAMPLER::Voice &voice = synth.voices[at];
  const Float gain =
    CORE::ENVELOPE::tick(voice.loudness, synth.loudness) * voice.level;
  const Whole reads = synth.clips[voice.clip].lanes.size();
  Float last = 0;
  for (Whole channel = 0; channel < synth.channels; ++channel) {
    if (channel < reads)
      last = SAMPLER::filtered(
        synth, voice, channel, SAMPLER::read(synth, voice, channel));
    lanes[channel][frame] += last * gain;
  }
  const Flag walking = SAMPLER::walk(voice);
  if (!walking || !CORE::ENVELOPE::sounding(voice.loudness)) ::end(synth, at);
}

}  // namespace

void SOUND::PLUGINS::SAMPLER::render(
  void *instance, AUDIO::PLUGIN::Sample *const *lanes, Whole frames,
  const AUDIO::PLUGIN::Event *events, Whole count) {
  CORE::BLOCK::denormals();
  auto &synth = *static_cast<Synth *>(instance);
  const auto land = [&synth](const AUDIO::PLUGIN::Event &event) {
    apply(synth, event);
  };
  CORE::BLOCK::Cursor cursor;
  Float peak = 0;
  for (Whole frame = 0; frame < frames; ++frame) {
    CORE::BLOCK::due(cursor, events, count, frame, land);
    if (CORE::BLOCK::due(synth.stride, STRIDE)) ::stride(synth);
    for (Whole channel = 0; channel < synth.channels; ++channel)
      lanes[channel][frame] = 0;
    for (Whole at = 0; at < POLYPHONY; ++at)
      if (synth.allocator.notes[at].sounding) ::sound(synth, at, lanes, frame);
    const Float heard = CORE::BLOCK::loudest(lanes, synth.channels, frame);
    if (heard > peak) peak = heard;
  }
  CORE::BLOCK::rest(cursor, events, count, land);
  CORE::BLOCK::publish(synth.meter, CORE::BLOCK::clipped(peak));
}
