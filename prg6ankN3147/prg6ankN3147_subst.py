from prg6ankN3147_rnd import MyPRNG, fib_gen
from datetime import datetime
from functools import wraps

class TypeError(Exception):
    pass

def shuffle_str_args(prng: MyPRNG, file_name: str = None):
    def decorator(func):

        if isinstance(func, type): # если функция это на самом деле класс, то нужно применить декоратор ко всем методам
            for name, method in func.__dict__.items(): #смотрим все методы класса
                if callable(method) and name != "__init__": #пропускаем инициализацию класса
                    setattr(func, name, decorator(method)) #применяем wrapper ко всем остальным методам
            return func #возвращаем функцию, которая на самом деле класс, для которого были продекорированы все методы кроем __init__

        else: # если func не класс а функция
            @wraps(func)
            def wrapper(*args, **kwargs): #позиционные и именнованные аргументы функции

                #если декоратор вызывается для отдлеьного метода класса, то args = [self, ...]
                #значит нам нало убрать self обработать остальные аргументы и вернуть self обратно для итого вызова func

                self = None
                if not isinstance(args[0], str):
                    self = args[0] #запомнимаем self
                    args = args[1:] #обрезаем args

                start_time = datetime.now().strftime("%d.%m.%Y %H:%M:%S")

                logs = [] #изменения
                shuffled_args = [] #для позиционных
                shuffled_kwargs = {} #для именнованных

                log_start = (
                    f"{datetime.now().strftime('%d.%m.%y %H:%M:%S')} "
                    f"{func.__name__}(args={args}, kwargs={kwargs})"
                )


                for i, arg in enumerate(args):
                    if isinstance(arg, str): #деократор должен работать только со строками
                        shuffled = prng.shuffle_str(arg) #с помощью метод из MyPRGN перемешиваем символы в строке
                        shuffled_args.append(shuffled) #запомняем довыми arg args
                        logs.append(
                            f"{datetime.now().strftime('%d.%m.%y %H:%M:%S')} "
                            f"{func.__name__}: аргумент {arg} (args[{i}]) заменен на '{shuffled}'"
                        )
                    else:
                        raise TypeError(f"Аргумент {arg} не является строкой")
                for key, value in kwargs.items():
                    if isinstance(value, str):
                        shuffled = prng.shuffle_str(value)
                        shuffled_kwargs[key] = shuffled
                        logs.append(
                            f"{datetime.now().strftime('%d.%m.%y %H:%M:%S')} "
                            f"{func.__name__}: аргумент {value} (kwargs['{key}']) заменен на '{shuffled}'"
                        )
                    else:
                        raise TypeError(f"Аргумент {value} не является строкой")

                if self != None: #если мы работали с методом класса
                    shuffled_args = [self, *shuffled_args]

                result = func(*shuffled_args, **shuffled_kwargs)
                # формирование полного лога
                full_log = log_start + f" -> {result}"
                if logs:
                    full_log += "\n" + "\n".join(logs)


                if file_name: #запись логов в узаканный файл
                    with open(file_name , 'a') as file:
                        file.write(full_log + '\n')
                else: #если имя файла не указано делаем логи в консоль
                    print(full_log + '\n')

                return result
            return wrapper
    return decorator


