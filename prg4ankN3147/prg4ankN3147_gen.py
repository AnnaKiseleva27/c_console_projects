import sys
import random
import secrets
import struct
from datetime import datetime, timedelta

# HH:MM:SS dd.mm.yyyy
def str_generation():
    start_date = datetime(1000, 1, 1)
    end_date = datetime(9999, 12, 31)
    random_date = start_date + timedelta(
        seconds=random.randint(0, int((end_date - start_date).total_seconds()))
    )
    return random_date.strftime("%H:%M:%S %d.%m.%Y")

# N, i - str index, o - int смещение, n - len(str), s - str
# n i o i o ... data n s n s ... data n_last s_last

def file_generation(file, N):

    index_list = list(range(N)) #массив индексов
    str_list = [str_generation() for i in range(N)] #массив строк

    random_data1 = random.randint(0, 100).to_bytes(4, byteorder='little') # произвольные данные
    random_data2 = random.randint(0, 100).to_bytes(4, byteorder='little') # произвольные данные

    offset_list = []
    current_offset = 4 + 6*N + 4

    for i in range(N): # высчитывание отступа для n_i
        offset_list.append(current_offset) 
        # Увеличиваем смещение на размер данных строки (4 байта на размер + сама строка)
        str_bytes = str_list[i].encode('utf-8') 
        current_offset += 4 + len(str_bytes)

    file.write((2*N).to_bytes(4, byteorder='little')) # запись cnt в начало файла

    for i in range(N):
        file.write(index_list[i].to_bytes(2, byteorder='little')) # запись индекса
        file.write(offset_list[i].to_bytes(4, byteorder='little')) # запись смещения

    file.write(random_data1) # произвольные данные 1

    for i in range(N-1): # запись длины строки и самой строки 
        str_bytes = str_list[i].encode('utf-8') 
        len_bytes = len(str_bytes).to_bytes(4, byteorder='little')
        file.write(len_bytes)
        file.write(str_bytes)
        print(index_list[i], offset_list[i], len_bytes, str_list[i])

    file.write(random_data2) # произвольные данные 2

    str_bytes = str_list[N-1].encode('utf-8') # запись последних элементов
    len_bytes = len(str_bytes).to_bytes(4, byteorder='little')
    file.write(len_bytes)
    file.write(str_bytes)
    print(index_list[N-1], offset_list[N-1], len_bytes, str_list[N-1])



def main():
    N = 0
    file = None

    if len(sys.argv) == 1:
        print("Ошибка: не введено имя файла")
        sys.exit(1)

    flag = False

    for i in range(1, len(sys.argv)):
        if flag: continue
        
        if sys.argv[i] == "-v":
            print("Анна Николавена Киселёва, гр N3147\nВариант 3-8-3-5")
            sys.exit(0)

        elif sys.argv[i] == "-n":
            if i + 1 >= len(sys.argv):
                print("Ошибка: после -n требуется число")
                sys.exit(1)
            try:
                N = int(sys.argv[i+1])
                file = sys.argv[i+2]
                flag = True
            except ValueError:
                print("Ошибка: после -n должно быть целое число")
                sys.exit(1)

        else:
            file = sys.argv[i]
            N = secrets.randbelow(991) + 10

    if file != None:
        try: 
            f = open(file, 'wb')
        except: 
            print("Ошибка: не удалось открыть файл")
            sys.exit(1)    
    else:
        print("Ошибка: укажите путь к файлу")
        sys.exit(1)       

    file_generation(f, N)
    

if __name__ == '__main__':
    main()