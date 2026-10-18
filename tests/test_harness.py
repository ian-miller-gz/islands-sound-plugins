import pathlib

import pytest

from test_shape import CLOSER, CONTEXT, OPENER, SPDX, stripped

HARNESSES = pathlib.Path(__file__).resolve().parent / 'harness'


def harnesses():
  return sorted(HARNESSES.glob('*.cpp'))


@pytest.mark.parametrize('path', harnesses(), ids=lambda path: path.name)
def test_a_harness_carries_the_licence_and_no_comment(path):
  lines = stripped(path.read_text()).splitlines()
  assert lines[0].startswith(SPDX)
  for line in lines[1:]:
    assert '/*' not in line
    if '//' in line:
      assert CLOSER.match(line), line


@pytest.mark.parametrize('path', harnesses(), ids=lambda path: path.name)
def test_a_harness_opens_no_namespace_outside_the_library(path):
  for line in stripped(path.read_text()).splitlines():
    opened = OPENER.match(line)
    if opened and opened.group(1):
      assert opened.group(1) == CONTEXT or opened.group(1).startswith(f'{CONTEXT}::'), line
