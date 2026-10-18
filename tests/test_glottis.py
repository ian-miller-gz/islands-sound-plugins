import pytest

CLOSED, SHUT, OPEN, APART = range(4)
EDGE = 0.6
TOLERANCE = 1e-3
BRIGHTER = 2
DIFFERING = 0.99


@pytest.fixture(scope='module')
def glottis(harness):
  return harness('glottis')


def test_middle_tension_opens_six_tenths_of_a_turn(glottis):
  assert glottis['edge'][0] == pytest.approx(EDGE, abs=TOLERANCE)


def test_a_pressed_voice_is_twice_as_bright_as_a_lax_one(glottis):
  dark, bright = glottis['bright']
  assert bright >= BRIGHTER * dark


def test_breath_sounds_in_the_open_phase_alone(glottis):
  breath = glottis['breath']
  assert breath[CLOSED] > 0 and breath[SHUT] == 0
  assert breath[OPEN] > 0 and breath[APART] >= DIFFERING * breath[OPEN]


def test_the_three_argument_settle_sounds_as_today_bit_for_bit(glottis):
  assert glottis['kept'] == [0]
