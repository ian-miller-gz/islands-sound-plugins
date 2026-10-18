// SPDX-License-Identifier: AGPL-3.0-or-later
#include "voice.internal.hpp"

namespace {
using namespace SOUND::PLUGINS;

constexpr Integer SHORTEST = 32;
constexpr Float HALF = 0.5f;
constexpr Whole OFF = 0;
constexpr Whole FORWARD = 1;
constexpr Whole SUSTAIN = 3;

struct Span {
  Integer low;
  Integer high;
};

auto spanned(const Float *rows, Whole one, Whole other, Float top) -> Span {
  const Float low = rows[one] < rows[other] ? rows[one] : rows[other];
  const Float high = rows[one] < rows[other] ? rows[other] : rows[one];
  return {
    Integer(low / SAMPLER::PERCENT * top),
    Integer(high / SAMPLER::PERCENT * top)};
}

auto held(Integer value, Span span) -> Integer {
  return value < span.low ? span.low : value > span.high ? span.high : value;
}

auto looping(Whole walk, Flag sustained) -> Flag {
  return walk == FORWARD || (walk == SUSTAIN && sustained);
}

}  // namespace

auto SOUND::PLUGINS::SAMPLER::region(
  const Synth &synth, const Clip &clip, Flag sustained) -> Region {
  const Whole count = frames(clip);
  if (count == 0) return {};
  const Float *rows = synth.rows;
  const Float top = Float(count - 1);
  const Span play = ::spanned(rows, START, END, top);
  const Span marks = ::spanned(rows, REPEAT, TURN, top);
  const Span loop = {::held(marks.low, play), ::held(marks.high, play)};
  const Flag reversed = rows[REVERSE] > ::HALF;
  Region region = {
    .origin = reversed ? play.high : play.low,
    .sign = reversed ? -1 : 1,
    .length = Float(play.high - play.low),
    .start = reversed ? play.high - loop.high : loop.low - play.low,
    .end = reversed ? play.high - loop.low : loop.high - play.low};
  const Integer span = region.end - region.start;
  region.walk = span < ::SHORTEST ? ::OFF : Whole(rows[LOOP]);
  region.wraps = ::looping(region.walk, sustained);
  const Float room = Float(reversed ? Integer(top) - loop.high : loop.low);
  const Float fade = rows[FADE] / PERCENT * Float(span);
  region.fade = region.wraps ? (fade < room ? fade : room) : 0;
  return region;
}
