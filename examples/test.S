section .data
out_cnt     db "too many arguments", 10, 0
out_char    db "not a number", 10, 0
out_fmt     db "argv[%02d] = %s", 10, 0
end_char: db " ", 10


section .bss
buf         resb 256
argc        resb 8
argv        resb 8

section .text

global main
extern printf

main:
    push    rbp
    mov     rbp, rsp

    mov [argc], rdi
    mov [argv], rsi
    xor r12, r12
loop:

    inc r12
    cmp [argc], r12
    jnz loop

    cmp r12, 2 ;проверка корректности введенных данных 
    jg error_cnt

real_num: 
    cmp byte [rsi],'0'
    jl out_char
    cmp byte [rsi], '9'
    ja out_char


    ;если все корректно то переход к проверки байтов на симметричность 

symmetrical:
    ; rsi - указатель на байт (число)
    
    ; Получаем первые 4 бита 
    mov rsi, [rdx]
    mov al, byte [rsi]        ; Загружаем байт
    and al, 0x0F        ; Получаем первые 4 бита (0100 1111) с маской (0000 1111)
    
    ;получаем вторые 4 бита
    mov bl, byte [rsi]        ; Загружаем байт снова
    shr bl, 4           ; Сдвигаем вправо на 4 бита
    and bl, 0x0F        ; Получаем вторые 4 бита (0000 0100) с маской (0000 1111)
    
    ; Сравниваем четверки битов 
    cmp al, bl           
    jne not_symmetrical   ; Если не равны, переходим к завершению
    
    ; Если триады симметричны, увеличиваем значение на 1
    inc byte [rsi]       ; Увеличиваем байт на 1
    jmp print_result      ; Переходим к выводу результата

not_symmetrical:
    ; Если триады не симметричны, просто завершаем
    jmp print_result      ; Вывод результата

print_result:
    ; Выводим результат
    mov rdi, rsi          ; Передаем указатель на измененный байт

    call printf           ; Вызываем printf
    jmp end

error_cnt:
    mov rdi, out_cnt      ; Указатель на сообщение об ошибке
    xor rax, rax
    call printf
    jmp end

error_char:
    mov rdi, out_char     ; Указатель на сообщение об ошибке
    xor rax, rax
    call printf
    jmp end

end:
    leave
    ret