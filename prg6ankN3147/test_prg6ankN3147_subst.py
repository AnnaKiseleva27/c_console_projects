from prg6ankN3147_rnd import MyPRNG, fib_gen
from prg6ankN3147_subst import TypeError, shuffle_str_args
import pytest

@pytest.fixture() # фикстура для создания экземпляра класса
def prng():
    gen = fib_gen()
    return MyPRNG(gen)

def test_dec_function(prng, tmp_path):
    """тест для проверки использования декоратора для функция"""
    file_path = tmp_path / "func_log.txt"

    @shuffle_str_args(prng, file_name=str(file_path))
    def test_func(password: str, username: str):
        return f"user: {username}, password: {password}"

    res = test_func('1234567', 'Anna')
    #полученные с помощзью ГПСЧ результат != исходному
    assert res != 'user: Anna, password: 1234567'

    assert file_path.exists()
    assert file_path.stat().st_size > 0


def test_dec_class_method(prng, tmp_path):
    file_path = tmp_path / "class_log.txt"
    """проверка использования декоратора для всего класса"""
    @shuffle_str_args(prng, file_name=str(file_path))
    class Boo:
        def __init__(self, x: str):
            self.x = x

        def get_x(self) -> str:
            return self.x

        def set_x(self, y: str) -> str:
            return self.x + y

        def sort_x(self, y: str):
            s = ''
            for i in sorted(y):
                s+=str(i)
            return s

    boo = Boo('Bottle of ')
    res = boo.set_x(y='Water') # возвращаем перемешанное слово Water
    res_2 = boo.sort_x(y = 'abcdefg') #возвращает отсортированную строку, тк декорируется аргумент
    assert res != 'Bottle of Water'
    assert res_2 == 'abcdefg'
    assert file_path.exists()
    assert file_path.stat().st_size > 0
    assert boo.get_x() == 'Bottle of '

def test_dec_class(prng, tmp_path):
     file_path = tmp_path / "class_method_log.txt"
     """проверка использованич декоратора для метода класса"""
     class Boo:
         def __init__(self, x: str):
             self.x = x

         def get_x(self) -> str:
             return self.x

         @shuffle_str_args(prng, file_name=str(file_path))
         def set_x(self, y: str) -> str:
             return self.x + y

         def sort_x(self, y: str):
             s = ''
             for i in sorted(y):
                 s+=str(i)
             return s

     boo = Boo('bottle of ')
     res_1 = boo.set_x('Water')
     res_2 = boo.sort_x(y = 'abcdefg')
     assert res_1 != 'Bottle_of_Water'
     assert res_2 != 'gfedcba'

     assert file_path.exists()
     assert file_path.stat().st_size > 0


def test_invalid_integer(prng, tmp_path):
    """тест на попытку передачи не строк, вызов TypeError"""
    file_path = tmp_path / "func_log.txt"

    @shuffle_str_args(prng, file_name=str(file_path))
    def test_func(a: str, b: str):
        return a + b

    res = test_func("123", "321")
    assert isinstance(res, str)

    with pytest.raises(TypeError):
        test_func(123, 321)

    assert file_path.exists()