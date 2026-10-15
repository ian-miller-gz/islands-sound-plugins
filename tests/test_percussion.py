import pytest

RATE = 48000
FADE = 480
TOLERANCE = 1e-4
WITHIN = 3
OPEN = 100
LIGHTER, LEVEL, HEAVIER = range(3)


@pytest.fixture(scope='module')
def percussion(harness):
  return harness('percussion')


def test_velocity_bends_through_its_curve(percussion):
  assert percussion['struck'] == pytest.approx([0.5, 0.8409, 0.0625, 1], abs=TOLERANCE)


def test_a_touch_lowers_a_soft_hit_by_octaves(percussion):
  assert percussion['touched'] == pytest.approx([1000, 500])


def test_an_open_choke_holds_full_gain(percussion):
  assert percussion['open'] == [OPEN]


def test_a_closed_choke_falls_to_silence_over_its_fade(percussion):
  assert abs(percussion['closed'][0] - FADE) <= 1
  assert percussion['after'] == [0]


def test_a_level_slope_keeps_the_sweep_bit_for_bit(percussion):
  assert percussion['differing'] == [0]


def test_a_slope_snaps_or_lingers_the_bend(percussion):
  assert percussion['drop'][0] == pytest.approx(0.5, abs=0.01)
  turns = percussion['turns']
  assert turns[LIGHTER] < turns[LEVEL] < turns[HEAVIER]


def test_a_kick_falls_silent_and_then_costs_nothing(percussion):
  first, silent, loud = percussion['kick']
  assert first == 1
  assert silent <= WITHIN
  assert loud == 0


def test_a_pair_is_poured_onto_the_lanes(percussion):
  assert percussion['single'] == pytest.approx([0.5, 0.125])
  assert percussion['double'] == pytest.approx([0.5, 0.5, -0.25])
  assert percussion['triple'] == pytest.approx([0.5, 0.5, -0.25, 0.125])


def test_a_pair_is_clipped_side_by_side(percussion):
  assert percussion['clip'] == pytest.approx([1, 1, 0])


def test_a_sample_is_written_as_before(percussion):
  assert percussion['mono'] == pytest.approx([0.5, -0.5, -0.5])
  assert percussion['over'] == pytest.approx([1, 1, 1])
