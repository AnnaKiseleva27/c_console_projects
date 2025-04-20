#include <stdlib.h>
#include <stdio.h>
#include <stdint.h> 
#include <stdbool.h>
#include <time.h>
#include <string.h>

//перевод числа в двоичную сс
void to_bin(uint8_t* num, int64_t size) {
    for (int64_t i = size - 1; i >= 0; i--) {
        for (int8_t j = 7; j >= 0; j--) {
            printf("%u", num[i] >> j & 1);
        }
        printf(" ");
    }
}
//функция проверки на симметричный байт
void symmetrical(uint8_t* num, uint64_t size) {
    for (uint64_t i = 0; i < size; i++){
        if (num[i] == 0)
            return;
        //сравнение младшей и страшей тетрады 
        for (uint8_t j = 0; j < 4; j++)
            if ((num[i] >> j & 1) == (num[i] >> (4 + j) & 1))
                return;
        num[i]++;
    }
} 
//проверка коректного ввода
bool real_num(const char* str) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (str[i] < '0' || str[i] > '9')
            return false;
    }
    return true;
}


int main(int argc, char* argv[]) {
    uint64_t n;
    if (argc == 2){
        if (!real_num(argv[1])){
            fprintf(stderr, "Ошибка: %s не является корректным числом. \n", argv[1]);
            return 0;
        }
        n = atoi(argv[1]);
    }
    if (argc == 1) {
        srand(time(NULL));
        n = rand();
    }
    if(argc > 2){
        fprintf(stderr, "Ошибка - необходимо ввести одно число\n");
        return 0;
    }
    
    to_bin((uint8_t*)&n, sizeof(n));
    symmetrical((uint8_t*)&n, sizeof(n));
    printf("\n");
    to_bin((uint8_t*)&n, sizeof(n));
    return EXIT_SUCCESS;
}
