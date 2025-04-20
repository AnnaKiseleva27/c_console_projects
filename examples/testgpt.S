section .data
    out_cnt     db "too many arguments", 10, 0
    out_char    db "not a number", 10, 0
    out_fmt     db "argv[%02d] = %d", 10, 0
    end_char    db " ", 10

section .bss
    buf         resb 256
    argc        resq 1       ; Используем resq для хранения 64-битного значения
    argv        resq 1       ; То же самое

section .text

global main
extern printf
extern atoi

main:
    push    rbp
    mov     rbp, rsp

    mov     [argc], rdi
    mov     [argv], rsi

    ; Считываем количество аргументов
    mov     rax, [argc]
    cmp     rax, 2
    jle     error_cnt       ; Если меньше 2, выводим ошибку

    ; Получаем указатель на первый аргумент argv[1]
    mov     rsi, [argv + 8] ; Указатель на argv[1]
    call check_symmetry

    jmp end

check_symmetry:
    ; Пробуем преобразовать строку в число, и проверяем на целостность
    mov     rax, rsi          ; rax указывает на строку
    call atoi                 ; Преобразуем строку в int
    mov     r8, rax           ; Сохраняем результат в r8 для дальнейшего использования

    ; Проверяем, симметричны ли младшие и старшие 4 бита
    mov     al, [r8]            ; Загружаем результат в al
    mul     al, 0x0F          ; Младшие 4 бита
    mov     bl, [r8]            ; Загружаем результат снова в bl
    shr     bl, 4             ; Сдвигаем вправо на 4 бита
    mul     bl, 0x0F          ; Старшие 4 бита
    cmp     al, bl            ; Сравниваем

    jne     not_symmetrical    ; Если не равны, переход к not_symmetrical

    ; Если симметричны, увеличиваем значение на 1
    inc     r8
    jmp print_result

not_symmetrical:
    ; Если не симметричны, ничего не делать
    jmp print_result

print_result:
    ;; Выводим результат
    mov     rdi, out_fmt     ; Указатель на формат
    mov     rsi, 1           ; Индекс argv[1] (для форматирования)
    mov     rdx, r8          ; Значение для вывода
    xor     rax, rax         ; Обнуляем rax для printf
    call    printf

    jmp end

error_cnt:
    mov     rdi, out_cnt      ; Указатель на сообщение об ошибке
    xor     rax, rax          ; Обнуляем rax для printf
    call printf
    jmp end

end:
    leave
    ret