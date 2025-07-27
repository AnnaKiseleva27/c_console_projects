# from email_validator import validate_email, EmailNotValidError
import re
# вызов ошибок FormatError TypeError UndoError RedoError
class FormatError(Exception):
    @classmethod
    def raise_error(cls, x):
        raise cls( f"Invalid email format in values: {x}")
class TypeError(Exception):
    @classmethod
    def raise_error(cls, x):
        raise cls(f"Unexpected type of {x}")

class UndoError(Exception):
    @classmethod
    def raise_error(cls):
        raise cls(f"Undo error")

class RedoError(Exception):
    @classmethod
    def raise_error(cls):
        raise cls(f"Redo error")
# две функции проверки корректного email адреса. Вторая с помощью библиотеки питоновской, она как то медленно работает
def re_is_valid_email(email): # c использованием шаблона (быстрее) но не совсем точноо
    regex = r'\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,7}\b'
    if not re.fullmatch(regex, email):
        return False
    return True

def is_valid_email(email): # с использованием email_validator (медленно)
    try:
        validate_email(str(email))
    except EmailNotValidError:
        return False
    return True

class MyDict(dict): #объявление класса
    def __init__(self, iter=None):
        super().__init__() # связь с родительским классом
        self.history = [] # сохранение исттории для undo и redo
        self.iter = 0 # счетчик для undo и redo
        if iter is not None:
            for k, v in iter: #добавление элементов
                self[k] = v
            self.current_state() #сохранение для истории

    def check(self, x): #проверка корректности введенного значения в словаре
        # if not isinstance(x, dict): #соответствие типу словаря
        #     TypeError.raise_error(x)

        for email in x.values(): #соответствие email виду
            if not re_is_valid_email(str(email)):
                FormatError.raise_error(x)

    def current_state(self): #смотрим что сейчас в словаре
        if self.iter + 1 < len(self.history): #если если сейчас элементов меньше чем было когда (бфло вызвано undo)
            self.history = self.history[:self.iter+1] #обрезаем
        self.history.append(dict(self)) #добавляем в историю
        self.iter = len(self.history)-1 #обновляем указатель

    def append(self, x):
        self.check(x) #проверка
        self.update(x) #добавление
        self.current_state() #обновление

    def undo(self):
        if self.iter <= 0: #уже некуда убавлять то вызов ошибки
            UndoError.raise_error()
        self.iter -= 1  # переход к предыдущему элементу
        self.clear()
        self.update(self.history[self.iter])
        print(f"Undo: {self}")

    def redo(self):
        if self.iter >= len(self.history)-1: #если в конце словаря
            RedoError.raise_error()
        self.iter+=1  #переход к следующему
        self.clear()
        self.update(self.history[self.iter])
        print(f"Redo: {self}")

# d = MyDict([('user1', 'a@b.com'), ('user2', 'kiselevanna27@mail.ru')])
# d.append({'user3': 'babana@gmail.ru'})
# d.append({'user4': 'bbbb@lala.com'})
# print(d)
