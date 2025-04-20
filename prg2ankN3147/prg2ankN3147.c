#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>
void print_matrix(int x, int y, short a[x][y]) { //печать матрицы 
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++)
            printf("%3hd ", a[i][j]); //%3  чтобы была красивая матрица 
        printf("\n");
    }
}
void fill_matrix(int x, int y, short a[x][y]){ //заполнение матрицы рандомными числами в сулчае отсутствия -m
    srand(time(NULL));
    for(int i = 0; i<x; i++)
        for(int j = 0; j<y; j++)
            a[i][j] = rand() % 20;
}
void move_matrix(int kx, int ky, int k, int x, int y, short a[x][y]){ //функция сдвига кольца 

    int temp1 = a[kx-k+1][ky]; //запоминаем верхний правый элемент 

    for(int i = kx-k+1; i < kx; i++) // правого столбца вверх
        a[i][ky]= a[i+1][ky];

    int temp2 = a[kx-k+1][ky-k+1]; //запоминаем верхний левый элемент

    for(int j = ky-k+1; j< ky; j++) // сдвиг верхнего ряда слево
        a[kx-k+1][j]=a[kx-k+1][j+1];

    a[kx-k+1][ky-1] = temp1;
    int temp3 = a[kx][ky-k+1]; // запоминаем нижний левый элемент 

    for(int i = kx; i > kx-k+1; i--) // сдвиг левого столбца вниз
        a[i][ky-k+1] = a[i-1][ky-k+1];

    a[kx-k+2][ky-k+1] = temp2;
    
    for(int j = ky; j > ky-k+1; j--) // сдвиг нижнего ряда вправо
        a[kx][j] = a[kx][j-1];
    a[kx][ky-k+2] = temp3;
}

int main(int argc, char *argv[]) {
    int x, y;
    int kx, ky, k, s;
    // Проверка запуска с переменной среды, включающей отладочный вывод.
    // Пример запуска с установкой переменной LAB2DEBUG в 1:
    // $ LAB2DEBUG=1 ./prg2abcNXXXXX 5 5
    char *DEBUG = getenv("LAB2DEBUG");
    if (DEBUG) {
        fprintf(stderr, "Включен вывод отладочных сообщений\n");
    }

    if ((argc != 4 || strcmp(argv[1], "-m")) && (argc != 3)) {
        fprintf(stderr, "Usage: %s [-m] введите значения матрицы тип данных short :(\n", argv[0]);
        return EXIT_SUCCESS;
    }
    
    if (argc == 4){ // если ввод данных вручную 
        x = atoi(argv[2]);
        y = atoi(argv[3]);
        short a[x][y];

        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                int value;
                if (scanf("%d", &value) != 1) { // некорректный ввод данных символ или строка
                    fprintf(stderr, "ERROR: inappropriate data type\n");
                    return 1;
                }
                if (value < SHRT_MIN || value > SHRT_MAX) { // некорректный ввод данных. число, превыщающее тип short
                    fprintf(stderr, "ERROR: inappropriate data type\n");
                    return 1;
                }
                a[i][j] = (short)value; //если все корректно то введенный символ нам подходит     
            }
        }

        fprintf(stderr, "введите координаты начала кольца, размер кольца, значение сдвига \n");
        scanf("%d %d %d %d", &kx, &ky, &k, &s);
        if(kx > x || ky > y || (ky-k+1) < 0 || (kx-k+1) < 0){ // проверка на корректность ввода значений кольца 
            fprintf(stderr, "ERROR: index out of matrix :(\n");
            return 1;
        }
        printf("\n");
        
        print_matrix(x, y, a);
        while(s--){ // сдвиг кольца на s элементов против часовой стрелки 
        move_matrix(kx, ky, k, x, y, a);
        }
        printf("\n");
        print_matrix(x, y, a);
    }
    
    if (argc == 3){ // если ввод данных автоматически
    x = atoi(argv[1]);
    y = atoi(argv[2]);

    fprintf(stderr, "введите координаты начала кольца, размер кольца, значение сдвига \n");
    scanf("%d %d %d %d", &kx, &ky, &k, &s);
    if(kx > x || ky > y || (ky-k+1) < 0 || (kx-k+1) < 0){ // проверка на корректность ввода значений кольца 
        fprintf(stderr, "ERROR: index out of matrix :(\n");
        return 1;
    }

    short a[x][y];
    fill_matrix(x, y, a);
    print_matrix(x, y, a);
    while(s--){ // сдвиг кольца на s элементов против часовой стрелки 
        move_matrix(kx, ky, k, x, y, a);
    }
    printf("\n");
    print_matrix(x, y, a);
    }
    return EXIT_SUCCESS;
}
