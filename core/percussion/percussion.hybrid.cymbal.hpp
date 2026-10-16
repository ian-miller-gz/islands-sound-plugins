// SPDX-License-Identifier: AGPL-3.0-or-later
#pragma once
#include "percussion.circuit.hpp"
#include "percussion.hybrid.loop.hpp"

namespace SOUND::PLUGINS::CORE::PERCUSSION::HYBRID {

namespace STROKE {

enum Stroke : Whole { CRASH, RIDE, STROKES };

}  // namespace STROKE

struct Cymbal {
  Loop loop;
  ENVELOPE::Envelope envelopes[STROKE::STROKES];
  ENVELOPE::Gate gate;
  Whole stroke = STROKE::CRASH;
  Float velocity = 0;
};

void build(Cymbal &cymbal, Whole rate);
void settle(Cymbal &cymbal, Float tune, Float decay, Whole rate);
void strike(Cymbal &cymbal, Whole stroke, Float velocity);
auto tick(Cymbal &cymbal) -> Float;
auto sounding(const Cymbal &cymbal) -> Flag;

}  // namespace SOUND::PLUGINS::CORE::PERCUSSION::HYBRID
