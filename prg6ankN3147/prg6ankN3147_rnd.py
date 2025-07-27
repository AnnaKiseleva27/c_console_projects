from abc import ABC, abstractmethod
from collections.abc import Iterator
#генератор псведослучаныйх чисел на основе последовательности Фибоначчи с запаздыванием
def fib_gen(j: int = 24, k: int = 55, mod: int = 2**32) -> Iterator[int]:
    a = [i for i in range(55)]
    i = k
    while True:
        num = (a[i - j] + a[i - k]) % mod
        if i < len(a):
            a[i] = num
        else:
            a.append(num)
        i += 1
        yield num

class PRNGBase(ABC):
    @abstractmethod
    def next_int(self) -> int:
        pass

    @abstractmethod
    def next_float(self) -> float:
        pass

    @abstractmethod
    def next_str(self) -> str:

        pass

    @abstractmethod
    def shuffle_str(self, src: str) -> str:
        pass

class MyPRNG(PRNGBase):
    def __init__(self, gen: Iterator):
        self.gen = iter(gen)

    def next_int(self) -> int:
        """Возвращает случайное целое число"""
        return( next(self.gen) )

    def next_float(self) -> float:
        """Возвращает случайное вещественное число в диапазоне от 0.0 до 1.0 (не включая)"""
        return( next(self.gen) / 2**32 )

    def next_str(self) -> str:
        """Возвращает случайную последовательность символов"""
        # получаемся Число из ГПСЧ по модулю 256 - код символа ascii
        max_len = 100
        string = []
        len_str = next(self.gen) % max_len  #ограничение длины генерированной стрки
        for i in range(len_str):
            num_ascii = next(self.gen) % 256
            string.append(chr(num_ascii))
        return(''.join(string))

    def shuffle_str(self, src: str) -> str:
        """Получает строку src и возвращает строку, содержащую символы из src, переставленные в случайном порядке"""
        string = list(src)
        for i in range(len(string)):
            j = next(self.gen) % (i+1) #символ исходной строки от 0 до i
            t = string[i] # меняем сиволы местами
            string[i] = string[j]
            string[j] = t
        return(''.join(string))





