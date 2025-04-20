section .data
    out_cnt     db "too many arguments", 10, 0
    out_char    db "not a number", 10, 0
    format      db "Updated byte: %d", 10, 0

section .bss
    argc        resb 8
    argv        resb 8
    num         resq 1  ; Для хранения преобразованного числа

section .text

global main
extern printf

main:
    push    rbp
    mov     rbp, rsp

    ; rdi содержит количество аргументов (argc)
    ; rsi содержит указатель на массив аргументов (argv)
    mov [argc], rdi
    mov [argv], rsi 

    ; Проверка, если argc больше 2
    cmp rdi, 2
    jg error_cnt

    ; Если argc <= 2, проверяем первый аргумент
    lea rsi, [rsi + 8]  ; Позиционируем указатель на argv[1]
    call convert_to_uint32

    ; Возвращаемся для печати результата
    jmp print_result

convert_to_uint32:
    xor rax, rax        ; Обнуляем rax для хранения результата (числа)
    xor r8, r8          ; Обнуляем r8, чтобы использовать для умножения
    mov rcx, 10         ; Устанавливаем base 10 для умножения

.next_digit:
    cmp byte [rsi], 10  ; Проверяем '\n'
    je end_convert       ; Выход из цикла, если конец строки
    cmp byte [rsi], '0'  ; Проверяем, меньше ли '0'
    jl error_char        ; Если меньше - ошибка
    cmp byte [rsi], '9'  ; Проверяем, больше ли '9'
    ja error_char        ; Если больше - ошибка

    sub byte [rsi], '0'  ; Преобразуем ASCII код в число (вычитаем код '0')
    mov bl, [rsi]        ; Сохраняем преобразованное значение в bl
    mul r8               ; Умножаем r8 на 10 (текущее значение) для корректного перемещения разрядов
    add rax, rbx          ; Добавляем новую цифру в rax
    inc rsi              ; Переходим к следующему символу
    jmp .next_digit      ; Переход к следующему символу

end_convert:
    mov [num], rax      ; Сохраняем результат в переменной num
    inc byte [num]      ; Увеличиваем значение на 1 (если нужно)
    ret

print_result:
    ; Выводим результат
    mov rdi, [num]      ; Передаем значение из num
    mov rsi, format     ; Формат строки для printf
    call printf         ; Вызываем printf
    jmp end

error_cnt:
    mov rdi, out_cnt    ; Указатель на сообщение об ошибке
    xor rax, rax
    call printf
    jmp end

error_char:
    mov rdi, out_char   ; Указатель на сообщение об ошибке
    xor rax, rax
    call printf
    jmp end

end:
    leave
    ret