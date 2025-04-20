#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdbool.h>
#include <stdlib.h>

typedef long long int ll;
typedef unsigned long ul;

typedef struct node_item_t {
    char* str;
    struct node_item_t *prev,
                       *next;

    ll index;
} node_item_t;

typedef struct node_t {
    struct node_item_t *head,
                       *tail;
    ll count;
} node_t;

node_t list = {NULL, NULL, 0}; // Инциализация списка как глобальной перемонной. Не хотел таскать как аргумент по всем функциям :)

// Создание узла списка
node_item_t *create_new_node (char *s);

// Считывание списка с файла
void file_input (FILE *fin);

// Вставка элемента в начало списка
void push_front (node_t *list, node_item_t *new_node);

// Вставка элемента в конец списка
void push_back (node_t *list, node_item_t *new_node);

// Удаление элемента из начала списка
void pop_front (node_t *list);

// Удаление элемента из конца списка
void pop_back (node_t *list);

void max_first(node_t *list);

// Вывод списка
void print_list (node_t *list);

// Распознавание команды и выполнение ее
void defining_a_command (char *command_name, char **credit_cards, ll size);

// Проверка по алгоритму Луна
bool condition_for_card (char *str);

// Сортировка списка (сделал просто потренироваться, упорядочивание по возрастанию)
void sort_list(node_t *list);

// Дамп списка в файл
void dump_list(node_t *list, char *filename);

int main(int argc, char *argv[]) {
    // Проверка запуска с переменной среды, включающей отладочный вывод.
    // Пример запуска с установкой переменной LAB4DEBUG в 1:
    // $ LAB4DEBUG=1 ./prg4abcNXXXXX test.txt
    char *DEBUG = getenv("LAB4DEBUG");
    if (DEBUG) {
        fprintf(stderr, "Включен вывод отладочных сообщений\n");
    }

    FILE *fin = stdin;

    if (argc == 2) {
        if (strcmp(argv[1], "-v") == 0){
            printf ("Бочковский Тимофей Алексеевич, гр. N3149\nВариант 2-4-11-6\n");
            return EXIT_SUCCESS;
        }
        else {
            fin = fopen(argv[1], "r+");
            if (!fin) {
                fin = fopen(argv[1], "a+");
            }
        }
    }
    else {
        printf("Ошибка: не задано имя файла.\n");
        return EXIT_FAILURE;
    }

    char *str = NULL, *command = NULL;                                                      // Создаем указатель str на считываемую строку.
                                                                                            // Указатель command используется после убирания
                                                                                            // возможных начальных пробелов.
    size_t str_size = 0;                                                                    // Размер считываемой строки в байтах.
    if (!fin) {
        printf("Ошибка: не удалось создать файл.\n");
        return EXIT_FAILURE;
    }
    file_input(fin);
    fin = stdin;
    char **credit_cards = NULL;
    ll cnt_word = 0;
    while (getline(&str, &str_size, fin) != -1) {
        int j = 0;
        while (str[j] == ' ') j++;                                                          // Пропускаем начальные пробелы
        command = &str[j];
        ll len = strlen(command), first_letter = 0, last_letter = 0;
        cnt_word = 0;
        credit_cards = malloc(sizeof(*credit_cards));                                // Создаем массив строк, в котором будут лежать номера карт,
                                                                                            // которые нужно добавить в список.
        if (!credit_cards) {
            printf("Ошибка: невозможно выделить память.\n");
            return EXIT_FAILURE;
        }
        char *command_name = NULL;                                                          // Название команды, которую нужно выполнить.
        for(ll i = 0; i < len; i++){
            if (command[i] == ' ' || command[i] == 10) {
                last_letter = i - 1;
                char *word = malloc((last_letter-first_letter+2)*sizeof(*word));
                if (!word) {
                    printf("Ошибка: не возможно выделить память.\n");
                    return EXIT_FAILURE;
                }
                strncpy(word, &command[first_letter], last_letter-first_letter+1);
                word[last_letter-first_letter+1] = '\0';
                if (cnt_word == 0) {
                    command_name = word;
                }
                else {
                    credit_cards = realloc(credit_cards, cnt_word * sizeof(*credit_cards));
                    if (!credit_cards) {
                        printf("Ошибка: не возможно перераспределить память.\n");
                        return EXIT_FAILURE;
                    }
                    credit_cards[cnt_word - 1] = word;
                }
                cnt_word++;
                while (command[i] == ' ' && i < len) i++;                                    // Находим начало следующего слова или, если оно было последнее, выходим из цикла.
                first_letter = i;
            }
        }
        defining_a_command(command_name, credit_cards, cnt_word-1);                          // Выполнение команды
    }
    for(int i = 0; i < cnt_word; i++) {
        free(credit_cards[i]);
    }
    free(credit_cards);
    fclose(fin);
    return EXIT_SUCCESS;
}

node_item_t *create_new_node (char *s) {
    node_item_t *new_node = malloc(sizeof(*new_node));
    if (!new_node) {
        printf ("Ошибка: не возможно выделить память.\n");
        return NULL;
    }
    else {
        new_node->str = s;
        new_node->next = new_node->prev = NULL;
    }
    return new_node;
}


void push_front (node_t *list, node_item_t *new_node) {
    if (list->head) {
        new_node->next = list->head;
        list->head->prev = new_node;
        list->head = new_node;
        list->count++;
        return;
    }
    else {
        list->head = list->tail = new_node;
        list->count++;
        return;
    }
}

void push_back (node_t *list, node_item_t *new_node) {
    if (list->head) {
        new_node->prev = list->tail;
        list->tail->next = new_node;
        list->tail = new_node;
        list->count++;
        return;
    }
    else {
        list->head = list->tail = new_node;
        list->count++;
        return;
    }
}

void pop_front (node_t *list) {
    if (!list->head) {
        return;
    }
    else {
        node_item_t *first_node = list->head;
        list->head = list->head->next;
        if (list->head) {
            list->head->prev = NULL;
        }
        else {
            list->tail = NULL;
        }
        list->count--;
        free(first_node);
        return;
    }
}

void pop_back (node_t *list) {
    if (!list->tail) {
        return;
    }
    else {
        node_item_t *last_node = list->tail;
        list->tail = list->tail->prev;
        if (list->tail) {
            list->tail->next = NULL;
        }
        else {
            list->head = NULL;
        }
        list->count--;
        free(last_node);
        return;
    }
}

void print_list (node_t *list) {
    node_item_t *curr_node = list->head;
    ul i = 0;
    while(curr_node) {
        printf("\e[1;91m[%lu]\e[m  \e[1;92m%s\e[m\n", i, curr_node->str);
        curr_node = curr_node->next;
        i++;
    }
    return;
}

void max_first(node_t *list) {
    if (list->count <= 1) {
        return;
    }
    node_item_t *curr_node = list->head;
    node_item_t *max_node = list->head;
    while (curr_node) {
        if (strcmp(curr_node->str, max_node->str) > 0) {
            max_node = curr_node;
        }
        curr_node = curr_node->next;
    }
    curr_node = list->head;
    while(curr_node) {
        node_item_t *next_node = curr_node->next;
        if (strcmp(curr_node->str, max_node->str) == 0) {
            if (curr_node->prev){
                curr_node->prev->next = curr_node->next;
            }
            if (curr_node->next) {
                curr_node->next->prev = curr_node->prev;
            }
            else {
                list->tail = curr_node->prev;
                if (list->tail){
                    list->tail->next = NULL;
                }
            }
            curr_node->next = list->head;
            curr_node->prev = NULL;
            list->head->prev = curr_node;
            list->head = curr_node;
        }
        curr_node = next_node;
    }
    return;
}

void defining_a_command (char *command_name, char **credit_cards, ll size) {
    if (strcmp(command_name, "push_front") == 0) {
        for(ll i = size - 1; i >= 0; i--) {
            if (condition_for_card(credit_cards[i])) {
                push_front(&list, create_new_node(credit_cards[i]));
            }
            else {
                printf ("Ошибка: карта с номером %s не была добавлена в спискок.\nДанная карта не прошла проверку корректности.\n", credit_cards[i]);
            }
         }
        return;
    }
    if (strcmp(command_name, "push_back") == 0) {
        for (ll i = 0; i < size; i++) {
            if (condition_for_card(credit_cards[i])) {
                push_back(&list, create_new_node(credit_cards[i]));
            }
            else {
                printf ("Ошибка: карта с номером %s не была добавлена в спискок.\nДанная карта не прошла проверку корректности.\n", credit_cards[i]);
            }
         }
        return;
    }
    if (strcmp(command_name, "pop_front") == 0) {
        if (size != 0) {
            printf("Ошибка: у данной команды нет аргументов.\n");
            return;
        }
        pop_front(&list);
        return;
    }
    if (strcmp(command_name, "pop_back") == 0) {
        if (size != 0) {
            printf("Ошибка: у данной команды нет аргументов.\n");
            return;
        }
        pop_back(&list);
        return;
    }
    if (strcmp(command_name, "print_list") == 0) {
        if (size != 0) {
            printf("Ошибка: у данной команды нет аргументов.\n");
            return;
        }
        print_list(&list);
        return;
    }
    if (strcmp(command_name, "max_first") == 0) {
        if (size != 0) {
            printf("Ошибка: у данной команды нет аргументов.\n");
            return;
        }
        max_first(&list);
        return;
    }
    if (strcmp(command_name, "sort") == 0) {
        if (size != 0) {
            printf("Ошибка: у данной команды нет аргументов.\n");
            return;
        }
        sort_list(&list);
        return;
    }
    if (strcmp(command_name, "dump") == 0) {
        if (size != 1) {
            printf("Ошибка: данная функции принимает 1 агрумент.\n");
            return;
        }
        dump_list(&list, credit_cards[size-1]);
        return;
    }
    printf("Ошибка: неподдерживаемая команда: '%s'.\n", command_name);
    return;
}

bool condition_for_card (char *str) {
    ll len = strlen(str), digit = 0, sum = 0;
    bool position = false;
    if (len != 19 || str[4] != '-' || str[9] != '-' || str[14] != '-'){
        return false;
    }
    for (int i = 0; i < len; i++) {
        if (i != 4 && i != 9 && i != 14 && (str[i] < '0' || str[i] > '9')) {
            return false;
        }
    }
    for (int i = len-1; i >= 0; i--) {
        if (str[i] == '-') {
            continue;
        }
        digit = str[i] - '0';
        if (position) {
            digit *= 2;
            if (digit > 9) {
                digit -= 9;
            }
        }
        sum += digit;
        position = !position;
    }
    return sum % 10 == 0;
}

void file_input(FILE *fin) {
    char **arr_s = NULL;
    ll len = 0;
    fseek(fin, 0, SEEK_END);
    len = ftell(fin);
    fseek(fin, 0, SEEK_SET);

    ul i = 0, n = 0, max_i = 0;
    char *s = NULL;

    while (ftell(fin) < len) {
        if (fread(&i, 2, 1, fin) != 1 || fread(&n, 2, 1, fin) != 1) {
            printf("Ошибка: некорректный формат файла.");
            abort();
        }
        s = malloc((n+1) * sizeof(*s));
        if (!s) {
            printf("Ошибка: невозможно выделить память.\n");
            free(s);
            abort();
        }
        if (fread(s, 1, n, fin) != n)    {
            printf("Ошибка: некорректный формат файла.");
            free(s);
            abort();
        }
        s[n] = '\0';
        max_i = (max_i < i) ? i : max_i;
        arr_s = realloc(arr_s, (max_i + 1) * sizeof(*arr_s));
        if (!arr_s) {
            printf("Ошибка: невозможно выделить память.\n");
            free(s);
            abort();
        }
        arr_s[i] = s;
    }
    // Теперь добавляем элементы в список
    for (ll i = max_i; i >= 0; i--) {
        if (arr_s[i] && condition_for_card(arr_s[i])) {
            push_front(&list, create_new_node(arr_s[i]));
        }
    }
    return;
}

void sort_list (node_t *list) {
    for (node_item_t *i = list->head; i->next; i = i->next) {
        for(node_item_t *j = list->head; j->next; j = j->next) {
            if (strcmp(j->str, j->next->str) > 0) {
                node_item_t *temp_node = malloc(sizeof(*temp_node));
                temp_node->next = j->next->next;
                temp_node->prev = j->prev;
                j->prev = j->next;
                j->next->next = j;
                j->next->prev = temp_node->prev;
                if (temp_node->prev) {
                    temp_node->prev->next = j->next;
                }
                else {
                    list->head = j->next;
                }
                j->next = temp_node->next;
                if (j->next) {
                    j->next->prev = j;
                }
                else {
                    list->tail = j;
                }
                j = j->prev;
            }
        }
    }
    return;
}

void dump_list (node_t *list, char *filename) {
    FILE *fout = fopen(filename, "w+");
    if (!fout) {
        fout = fopen(filename, "a+");
    }
    if(!fout) {
        printf("Ошибка: не удалось создать файл.\n");
    }
    for (node_item_t *i = list->head; i; i = i->next) {
        fprintf(fout, "%20p %20p %20p %25s\n", i->prev, i, i->next, i->str);
    }
    return;
}
