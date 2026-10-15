import pytest

BOUND = 1
QUIETEST = 0.25
LOUDEST = 0.42
SLOW = 0.8
ONE, TWO, APART = range(3)


@pytest.fixture(scope='module')
def modulator(harness):
  return harness('modulator')


def test_every_value_is_held_to_the_unit_swing(modulator):
  assert modulator['most'][0] <= BOUND


def test_the_walk_rests_at_about_a_third(modulator):
  assert QUIETEST <= modulator['rms'][0] <= LOUDEST


def test_the_walk_is_slow_at_a_tenth_of_its_time_constant(modulator):
  assert modulator['correlation'][0] >= SLOW


def test_one_seed_walks_one_sequence(modulator):
  assert modulator['differing'] == [0]


def test_two_seeds_part_at_the_first_tick(modulator):
  first = modulator['first']
  assert first[APART] == 1 and first[ONE] != first[TWO]
