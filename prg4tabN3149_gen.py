import secrets
import sys
import random
#!/usr/bin/python3

def condition_for_card(credit_card):
    credit_card = credit_card.replace('-', '')
    position = False
    sum_digits = 0
    for i in range(len(credit_card) - 1, -1, -1):
        digit = int(credit_card[i])
        if position:
            digit *= 2
            if digit > 9:
                digit -= 9
        sum_digits += digit
        position = not(position)
    return sum_digits % 10 == 0

def generator(fin, N):
    index_list = list(range(N))
    random.shuffle(index_list)

    for i in range(N):
        fin.write(index_list[i].to_bytes(2, byteorder='little'))
        credit_card = '-'.join([''.join([str(random.randint(0, 9)) for _ in range(4)]) for _ in range(4)])
        while not condition_for_card(credit_card):
            credit_card = '-'.join([''.join([str(random.randint(0, 9)) for _ in range(4)]) for _ in range(4)])
        credit_card_byte = credit_card.encode('utf-8')
        len_byte = len(credit_card_byte).to_bytes(2, byteorder='little')
        fin.write(len_byte)
        fin.write(credit_card_byte)
        print(index_list[i], len_byte, credit_card)

def main():
    filename = None
    N = -1
    if len(sys.argv) == 1:
        print("Ошибка: не указано название файла")
        sys.exit()
    else:
        i = 1
        while i < len(sys.argv):
            if sys.argv[i] == '-v':
                print("Бочковский Тимофей Алексеевич, гр. N3149\nВариант 2-4-11-6")
                sys.exit(0)
            elif sys.argv[i] == '-n':
                if i+1 < len(sys.argv):
                    try:
                        N = int(sys.argv[i + 1])
                        i += 1
                    except ValueError:
                        print("Ошибка: аргумент после -n должен быть числом!")
                        sys.exit(1)
                else:
                    print("Ошибка: после -n должено быть число!")
                    sys.exit(1)
            else:
                filename = sys.argv[i]
            i += 1
    if filename != None:
        try:
            fin = open(filename, 'wb')
        except IOError:
            print("Ошибка: неудалось записать в файл")
            sys.exit(1)
    else:
        print("Ошибка: укажите название файла")
        sys.exit(1)
    if N == -1:
        N = random.randint(10, 1000)
    generator(fin, N)
    pass

if __name__ == '__main__':
    main()

