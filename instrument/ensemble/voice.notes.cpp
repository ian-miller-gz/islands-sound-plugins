// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND;

constexpr Whole KINDS = AUDIO::PLUGIN::Event::PROGRAM + 1;

void wake(ENSEMBLE::Synth &synth) {
  const Flag ringing = CORE::ENVELOPE::sounding(synth.gate);
  for (ENSEMBLE::Key &key : synth.keys) {
    key.target = 0;
    if (!ringing) key.level = 0;
  }
  CORE::ENVELOPE::strike(synth.gate, synth.envelope, CORE::ENVELOPE::FULL);
}

void release(ENSEMBLE::Synth &synth, const AUDIO::PLUGIN::Event &event) {
  if (event.index >= CORE::PHASE::PITCHES) return;
  ENSEMBLE::Key &key = synth.keys[event.index];
  if (!key.held) return;
  key.held = false;
  if (--synth.held > 0)
    key.target = 0;
  else
    CORE::ENVELOPE::lift(synth.gate);
}

void press(ENSEMBLE::Synth &synth, const AUDIO::PLUGIN::Event &event) {
  if (event.value <= 0) return release(synth, event);
  if (event.index >= CORE::PHASE::PITCHES) return;
  ENSEMBLE::Key &key = synth.keys[event.index];
  if (key.held) return;
  if (synth.held == 0) ::wake(synth);
  key.held = true;
  key.target = 1;
  ++synth.held;
}

void steer(ENSEMBLE::Synth &synth, const AUDIO::PLUGIN::Event &event) {
  if (event.index >= ENSEMBLE::PARAMETERS) return;
  synth.rows[event.index] =
    CORE::TABLE::clamped(ENSEMBLE::SHEET, event.index, event.value);
  ENSEMBLE::settle(synth);
}

void ignore(ENSEMBLE::Synth &, const AUDIO::PLUGIN::Event &) {}

using Handler = void (*)(ENSEMBLE::Synth &, const AUDIO::PLUGIN::Event &);

constexpr Handler HANDLERS[KINDS] = {press,  release, steer,
                                     ignore, ignore,  ignore};

}  // namespace

void SOUND::ENSEMBLE::apply(Synth &synth, const AUDIO::PLUGIN::Event &event) {
  if (event.kind < ::KINDS) ::HANDLERS[event.kind](synth, event);
}
