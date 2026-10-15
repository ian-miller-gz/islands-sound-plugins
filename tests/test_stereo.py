import math

import pytest

TOLERANCE = 1e-5
ROOT = math.sqrt(2)
POWER = 2
PLACES = 41


@pytest.fixture(scope='module')
def stereo(harness):
  return harness('stereo')


def test_a_centred_source_keeps_its_level_on_each_lane(stereo):
  assert stereo['centre'] == pytest.approx([1, 1], abs=TOLERANCE)


def test_a_side_takes_the_whole_power(stereo):
  assert stereo['left'] == pytest.approx([ROOT, 0], abs=TOLERANCE)
  assert stereo['right'] == pytest.approx([0, ROOT], abs=TOLERANCE)


def test_the_power_is_equal_at_every_place(stereo):
  assert len(stereo['power']) == PLACES
  assert stereo['power'] == pytest.approx([POWER] * PLACES, abs=TOLERANCE)


def test_a_place_beyond_the_field_is_held_to_its_edge(stereo):
  assert stereo['beyond'] == stereo['left']


def test_a_key_is_placed_about_middle_c_by_width(stereo):
  assert stereo['place'] == pytest.approx([0, 1, -0.5, 1, 0.5], abs=TOLERANCE)


def test_one_lane_takes_the_mean(stereo):
  assert stereo['single'] == pytest.approx([0.125])


def test_two_lanes_take_the_sides(stereo):
  assert stereo['double'] == pytest.approx([0.5, -0.25])


def test_further_lanes_take_the_mean(stereo):
  assert stereo['quad'] == pytest.approx([0.5, -0.25, 0.125, 0.125])


def test_a_source_adds_by_its_gains(stereo):
  assert stereo['add'] == pytest.approx([0.5, -0.25 + 0.5 * ROOT], abs=TOLERANCE)


def test_the_magnitude_is_the_loudest_side(stereo):
  assert stereo['magnitude'] == pytest.approx([0.5])
