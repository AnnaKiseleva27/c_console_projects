#include <stdlib.h>
#include <stdio.h>
// Добавьте необходимые заголовочные файлы
#include <time.h>

// Структура S
typedef struct S {
    unsigned short a;
    long b;
} S;

void init_array(S* arr, int size) {
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        arr[i].a = rand() % 65536; 
        arr[i].b = rand() % 2147483648; 
    }
}

void print_array(S* arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d %zu \n", arr[i].a, arr[i].b);
    }
    printf("\n");
}

int comp(const void* a, const void* b) {
    S* s1 = (S*)a;
    S* s2 = (S*)b;

    return (s2->b >> 16) - (s1->b >> 16);
}

void sort_array(S* arr, int size, int (*comp)(const void*, const void*)) {
    qsort(arr, size, sizeof(S), comp);
}

int main(int argc, char* argv[]) {

    size_t arr_size = atoi(argv[1]);
    S* arr = (S*)malloc(arr_size * sizeof(S));
    // Заполнить массив случайными значениями
    init_array(arr, arr_size);
    // 4. Вывести массив на экран
    print_array(arr, arr_size);
    // 5. Отсортировать массив по заданному критерию
    sort_array(arr, arr_size, comp);
    // 6. Вывести отсортированный массив на экран
    print_array(arr, arr_size);

    return 0;
}