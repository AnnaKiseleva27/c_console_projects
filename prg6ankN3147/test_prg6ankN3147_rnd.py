from prg6ankN3147_rnd import MyPRNG, fib_gen
import pytest
@pytest.fixture()
def prng():
    gen = fib_gen()
    return MyPRNG(gen)

def test_shuffled_string(prng):
    """Проверка работы метода класса по перемешиванию строк с помощью ГПСЧ"""
    shuffle_str = ['abcdefg', '12345667', 'barabulka']
    for i in shuffle_str:
        shuffle = prng.shuffle_str(i)
        assert shuffle != i
    assert prng.shuffle_str('aaa') == 'aaa'
    assert prng.shuffle_str('') == ''

def test_next_int(prng):
    """проверка метода вывода следующего целого числа"""
    res = prng.next_int()
    assert isinstance(res, int)

def test_next_float(prng):
    """проверка метода выовда следующего вещественного числа"""
    res = prng.next_float()
    assert isinstance(res, float)

def test_next_str(prng):
    """Получает строку src и возвращает строку, содержащую символы из src, переставленные в случайном порядке"""
    s = set()
    for i in range(10):
        res = prng.next_str()
        assert len(res) <= 100
        assert isinstance(res, str)
        s.add(res)
    assert len(s) == 10


