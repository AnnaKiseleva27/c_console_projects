from prg5ankN3147 import FormatError, UndoError, RedoError, MyDict, re_is_valid_email
import pytest
#корректные email адреса
valid_emails = [
    {'user1': 'a@b.com'},
    {'user2': 'kiselevanna27@mail.ru'},
    {'user3': 'marinovannye@ogurtsi.com'},
    {'user4': 'babana@gmail.ru'}
]
#некорректные email адреса
invalid_emails = [
    {'user5': 'dddd@gcom'},
    {'user6': 'dddom'},
    {'user7': '!@#@$@$'},
    {'user8': 'a b c '}
]
#проверка функции соответствия email адресу
@pytest.mark.parametrize("maybe_email, expected_result", [
    ("kiselevanna27@mail.ru", True),
    ("a@b.com", True),
    ("aba", False),
    ("cd.com", False),
])
def test_re_is_valid_email(maybe_email, expected_result):
    """тест на проверку регулярного выражения"""
    assert re_is_valid_email(maybe_email) is expected_result


def test_valid_append():
    """тест на проверку добавления корректного email"""
    d = MyDict()
    for email in valid_emails:
        d.append(email)

    for email in valid_emails:
        for k, v in email.items():
            assert k in d
            assert d[k] == v

def test_invalid_append():
    """тест на проверку добавления некорректного email"""
    d = MyDict()
    for email in invalid_emails:
        with pytest.raises(FormatError):
            d.append(email)
    assert len(d) == 0

def test_valid_undo_redo():
    """тест на проверку работающей функции undo"""
    d = MyDict()
    for email in valid_emails:
        d.append(email)
    d.undo()
    assert 'user4' not in d
    d.redo()
    assert 'user4' in d

def test_invalid_undo():
    """тест на проверку неработающей функции undo"""
    d = MyDict()
    with pytest.raises(UndoError):
        d.undo()

def test_invalid_redo():
    """тест на проверку неработающей функции redo"""
    d = MyDict()
    with pytest.raises(RedoError):
        d.redo()

