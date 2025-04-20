section .data                           
    too_many_args db "to many arguments", 0xA, 0 ; сообщение об ошибке 1
    too_many_args_len equ $ - too_many_args ; длина сообщения об ошибке 1
    not_a_number db "not a number", 0xA, 0 ; сообщение об ошибке 2
    not_a_number_len equ $ - not_a_number ; длина сообщения об ошибке 2
    newline : db 10 ; переход на новую строку

section .bss
    argument     resb 256 ; хранение аргументов строки           
    arg_len      resb 8 ; длина рагументов строки            
    bin_str      resb 65 ; для вывода в 2 сс           
    rand         resq 1 ; рандомное число         
    buf          resb 10 ; буфер для числа, используется в функции проверки на симметричность 

section .text
    global _start

_start:

    mov rsi, [rsp] ; считывааем количество аргументов с консоли         
    cmp rsi, 2 ; сравниваем с 2            
    ja  .too_many_args ; если больше 2, то вывод сообщения об ошибке    
    jl  .rand_num ; если меньше то на вход программы подается рандомное число         

    mov rdi, [rsp + 16] ; переходит ко argv[1]      
    mov rsi, argument ; копируем для аргумента            
    mov rcx, 256 ; длина копирования 256           
    call copy ; функция копирования троки          
    mov [arg_len], rax ; копирует длину аргумента строки     

    mov rsi, argument  ; указатель на число          
    mov rcx, [arg_len] ; длина число     
    call is_number ; проверка число на отсутствие символов 
    test rax, rax ; результат работы функции is_number          
    jz .not_a_number ; если не число вывод сообщения об ошибке 

    mov rsi, argument ; укзатель на число          
    call convert_to_int ; преобразование число в строку               
    mov rbx, rax ; переносим число в rbx           

    jmp .main_function ; вывод + основная функция 

.not_a_number:
    mov rax, 1 ; вывод            
    mov rdi, 1 ; на консоль             
    mov rsi, not_a_number ; выводим сообщение об ошибке 
    mov rdx, not_a_number_len ; длина сообщения 
    syscall
    jmp .end ; завершение программы                

.too_many_args:
    mov rax, 1  ; вывод            
    mov rdi, 1  ; на консоль            
    mov rsi, too_many_args ; выводим сообщение об ошибке 
    mov rdx, too_many_args_len ; длина сообщения
    syscall
    jmp .end ; завершение программы             

.rand_num: ; рандомное число
    rdtsc  ; таймер процессора                 
    xor rdx, rdx ; обнуление            
    mov rcx, 18446744073709551615 ; макс число 2 в 64 степени  
    div rcx  ; получаем рандомное число               
    mov rbx, rdx ; сохраняем в rbx           
    mov [rand], rdx ;          

.main_function:

    mov rsi, rbx ; число для перевода в 2сс           
    mov rdi, bin_str ; указатель на массив с 2 сс       
    call uint_to_bin ; вызов функции перевода       

    mov rax, 1 ; вывод             
    mov rdi, 1 ; на консоль             
    mov rsi, bin_str ; выводим число в 2 сс       
    mov rdx, 64 ; длина 64 бита             
    syscall

    mov rax, 1
    mov rdi, 1
    mov rsi, newline ; вывод  на консоль \n
    mov rdx, 1
    syscall

    call check_symmetry ; функция проверки байтов на симметричность
    mov rsi, rbx ; сохраняем число           
    mov rdi, bin_str ; указатель на массив 2 сс       
    call uint_to_bin ; преобразование в 2 сс       

    mov rax, 1              
    mov rdi, 1              
    mov rsi, bin_str ; вывод результата в 2 сс        
    mov rdx, 64             
    syscall

    mov rax, 1
    mov rdi, 1
    mov rsi, newline ; вывод \n
    mov rdx, 1
    syscall

.end:
    mov rax, 60  ; exit          
    xor rdi, rdi ; код возврата 0
    syscall

check_symmetry: ; функци проверки на симметричность 
    xor rcx, rcx ; счетчик            
    xor r10, r10 ; флаг           
    mov [buf], rbx ; число в буфер 

    test rbx, rbx ; равно ли число 0 
    jz end_function ; елси да, то конец 
    
    bsf rcx, rbx ; ищем первый бит              
    bsr rdx, rbx ; индекс последнего бита                
    cmp rcx, rdx ; сравнение индексов                 
    je end_function ; если равны то число симметрично 

check_loop:
    mov al, byte [buf + rcx] ; текущий байт 
    mov cl, al ; копируем байт в  cl 
    and cl, 0x0F ; с помощью маски берем младшие 4 ьита 
    shr al, 4 ; сдвиг на 4 впрово 
    xor al, 0x0F ; первые 4 бита байта 
    cmp al, cl ; сравнение 
    je byte_is_symmetric ; если равны, то байт - симметричный 

check_continue: 
    inc rcx ; увеличиваем счетчик 
    cmp rcx, 8 ; проверили ли все байты 
    jl check_loop ; если нет то продолжаем 

check_end:
    cmp r10, 1 ; флаг если есть симметричные байты 
    je increment_number ; увеличивааем чсило
    jmp end_function ; конец функции 

byte_is_symmetric:
    mov r10, 1 ; симметричный байт 
    jmp check_continue ; перехход к след байту 

increment_number:
    inc dword [buf] ; увеличиваем число 
    mov rbx, [buf] ; возвращаем число в регистр 

end_function: ; конец функции 
    ret

uint_to_bin: ; функция перевода в 2 сс 
    mov rcx, 64 ; счетчик на 64 бита 
    mov rax, rsi ; копирование числа 
.loop:
    dec rcx ; уменьшаем счетчик 
    test rax, 1 ; сравнение  с 1 
    mov byte [rdi + rcx], '0' ; бит = 0
    jz .skip ; если равен 0, то след шаг 
    mov byte [rdi + rcx], '1' ; иначе 1 
.skip:
    shr rax, 1 ; сдвиг вправо на 1
    test rcx, rcx ; равен ли счетчик 0 
    jnz .loop ; продолжаем если счетчик не 0 
    mov byte [rdi + 64], 0 ; \0
    ret

convert_to_int: ; подготовительные децствия для перевода строки в число
    xor rax, rax

convert_to_int_lp:
    movzx rcx, byte [rsi] ; текущий символ 
    cmp rcx, 0 ; конец строки ли 
    je convert_to_int_end ; если да - завершение 
    sub rcx, '0' ; прибавляем код 0 
    imul rax, rax, 10 ; след разряд 
    add rax, rcx ; добавляем цифру
    inc rsi ; след символ
    jmp convert_to_int_lp

convert_to_int_end: ; завершение функции 
    ret

copy: ; функция копирования строки чтобы не портить исходные данные при обработке 
    xor rax, rax ; счетчик символов
.copy_lp:
    cmp byte [rdi + rax], 0 ; конец ли строки 
    je .copy_end ; если конец, то завершение функции копирования 
    mov bl, [rdi + rax] ; считываем символ
    mov [rsi + rax], bl ; переносим символ
    inc rax ; увеличиваем счетчик 
    jmp .copy_lp ; возвращаемся к след символу
.copy_end: ; конец функции копирования 
    ret

is_number: ; подготовительные действия для проверки чсило или не число
    xor rax, rax ; флаг результата            
    xor rbx, rbx ; индекс тек симвоола
 
.is_number_loop: 
    cmp rbx, rcx ; конец ли строки           
    jge .is_number_end ; завершение функции, ретерн
    mov dl, byte [rsi + rbx] ; считываем тек символ 
    cmp dl, 0  ; конец ли строки            
    je .is_number_end ; завершаем программу
    cmp dl, '0' ; сравниваем с кодом нуля             
    jl .not_a_number_return ; если меньше то не число 
    cmp dl, '9' ; сравниваем с кодом 9              
    ja .not_a_number_return ; если больш ето не число 
    inc rbx ; след символ                
    jmp .is_number_loop

.is_number_end: 
    mov rax, 1  ; итог функции проверки на строки на число - число           
    ret

.not_a_number_return:
    xor rax, rax  ; итог - строка не число         
    ret
