import pathlib
import re

import pytest

ROOT = pathlib.Path(__file__).resolve().parents[1]
TYPES = {'instrument', 'effect', 'notes', 'control', 'device', 'modular', 'emulator', 'core'}
VOICED = {'instrument', 'emulator'}
VOICINGS = {'mono', 'poly'}
LINES = 100
CONTEXT = 'SOUND::PLUGINS'
SEAM = 'SOUND::PLUGIN'
CARTRIDGE = {'../../../plugin.hpp', '../../../inventory.hpp'}
ENGINE = {'threads.hpp', 'island/audio.hpp', 'island/midi.hpp', 'xmmintrin.h'}
SPDX = '// SPDX-License-Identifier: '

WORD = re.compile(r'[a-z]+')
FILE = re.compile(r'[a-z]+(\.[a-z]+)*\.(cpp|hpp)')
OPENER = re.compile(r'^\s*namespace\s*([A-Za-z_:]*)\s*\{', re.M)
CLOSER = re.compile(r'^\}  // namespace( \S+)?$')
QUALIFIED = re.compile(r'(?<![\w:])SOUND::\w+')
INCLUDE = re.compile(r'^#include\s*([<"])([^>"]+)[>"]', re.M)
LITERAL = re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'')
OFFER = re.compile(r'\{\s*\.name\s*=')
TYPED = re.compile(r'\.type\s*=\s*"([^"]*)"')
VOICING = re.compile(r'\.voicing\s*=\s*(?:SOUND::PLUGIN::([A-Z]+)|"([^"]*)")')
INS = re.compile(r'\.ins\s*=\s*\{')


def sources():
  return sorted(
    path for path in ROOT.rglob('*')
    if path.suffix in ('.cpp', '.hpp') and not path.relative_to(ROOT).parts[0].startswith('.')
    and path.relative_to(ROOT).parts[0] != 'tests')


def families():
  return sorted({path.parent for path in sources()})


def plugins():
  return [family for family in families() if family.parent.name != 'core']


def named(path):
  return str(path.relative_to(ROOT))


def braced(text, start):
  depth = 0
  for at in range(start, len(text)):
    depth += {'{': 1, '}': -1}.get(text[at], 0)
    if depth == 0:
      return text[start:at + 1]
  return text[start:]


def offers(family):
  found = []
  for path in sorted(family.glob('*.cpp')):
    text = path.read_text()
    for match in OFFER.finditer(text):
      body = braced(text, match.start())
      if '.surface' in body:
        found.append(body)
  return found


def noted(family):
  for path in sorted(family.iterdir()):
    text = path.read_text()
    for match in INS.finditer(text):
      if 'Port::NOTES' in braced(text, match.end() - 1):
        return True
  return False


def stripped(text):
  return LITERAL.sub('""', text)


@pytest.mark.parametrize('path', sources(), ids=named)
def test_a_source_sits_at_type_name_file(path):
  parts = path.relative_to(ROOT).parts
  assert len(parts) == 3
  assert parts[0] in TYPES
  assert WORD.fullmatch(parts[1])
  assert FILE.fullmatch(parts[2])


@pytest.mark.parametrize('family', families(), ids=named)
def test_a_family_holds_its_root_aggregate(family):
  assert (family / f'{family.name}.hpp').is_file()


@pytest.mark.parametrize('path', sources(), ids=named)
def test_a_file_stays_within_the_size_law(path):
  assert len(path.read_text().splitlines()) <= LINES


@pytest.mark.parametrize('path', sources(), ids=named)
def test_no_comment_but_the_licence_and_the_closers(path):
  lines = stripped(path.read_text()).splitlines()
  assert lines[0].startswith(SPDX)
  for line in lines[1:]:
    assert '/*' not in line
    if '//' in line:
      assert CLOSER.match(line), line


@pytest.mark.parametrize('path', sources(), ids=named)
def test_every_namespace_opened_is_the_library_or_nested_in_it(path):
  text = stripped(path.read_text())
  stack = []
  for line in text.splitlines():
    opened = OPENER.match(line)
    if opened and not line.lstrip().startswith('using'):
      name = opened.group(1)
      if not name:
        stack.append('')
      elif stack:
        assert stack[-1] == '' or stack[-1].startswith(CONTEXT), line
        stack.append(stack[-1] and f'{stack[-1]}::{name}')
      else:
        assert name == CONTEXT or name.startswith(f'{CONTEXT}::'), line
        stack.append(name)
    elif CLOSER.match(line):
      stack.pop()
  assert not stack


@pytest.mark.parametrize('path', sources(), ids=named)
def test_every_sound_spelling_is_the_library_or_the_seam(path):
  for spelled in QUALIFIED.findall(stripped(path.read_text())):
    assert spelled in (CONTEXT, SEAM), spelled


@pytest.mark.parametrize('path', sources(), ids=named)
def test_the_include_rule(path):
  family = path.parent
  core = family.parent.name == 'core'
  for bracket, target in INCLUDE.findall(path.read_text()):
    if bracket == '<':
      assert '.' not in target or target in ENGINE, target
      continue
    parts = target.split('/')
    if target in CARTRIDGE or '/' not in target:
      assert target in CARTRIDGE or (family / target).is_file(), target
    elif core:
      assert len(parts) == 3 and parts[0] == '..' and parts[2] == f'{parts[1]}.hpp', target
      assert (family / target).is_file(), target
    else:
      assert parts[:3] == ['..', '..', 'core'] and len(parts) == 5, target
      assert parts[4] == f'{parts[3]}.hpp' and (family / target).is_file(), target


@pytest.mark.parametrize('family', plugins(), ids=named)
def test_a_plugin_offers_itself(family):
  assert offers(family)


@pytest.mark.parametrize('family', plugins(), ids=named)
def test_an_offer_states_its_directory_as_its_type(family):
  for offer in offers(family):
    assert TYPED.findall(offer) == [family.parent.name], offer


@pytest.mark.parametrize('family', plugins(), ids=named)
def test_an_instrument_or_a_module_states_a_voicing(family):
  kind = family.parent.name
  voiced = kind in VOICED or (kind == 'modular' and noted(family))
  for offer in offers(family):
    stated = [(upper or word).lower() for upper, word in VOICING.findall(offer)]
    if voiced:
      assert len(stated) == 1 and stated[0] in VOICINGS, offer
    else:
      assert not stated, offer
