import pytest

SLOTS = 16
BOUND = 1
SWING = 0.2
FIELDS = ('pitch', 'cutoff', 'time')


@pytest.fixture(scope='module')
def voice(harness):
  return harness('voice')


def test_every_slot_is_drawn(voice):
  for field in FIELDS:
    assert len(voice[field]) == SLOTS, field


def test_every_field_lies_within_the_unit_swing(voice):
  for field in FIELDS:
    assert all(-BOUND <= value <= BOUND for value in voice[field]), field


def test_the_pitch_trims_differ_and_swing_wide(voice):
  pitches = voice['pitch']
  assert len(set(pitches)) > 1
  assert sum(abs(value) for value in pitches) / len(pitches) > SWING


def test_one_seed_draws_one_set(voice):
  assert voice['differing'] == [0]


def test_two_seeds_part_at_the_first_slot(voice):
  assert voice['apart'] == [1]
