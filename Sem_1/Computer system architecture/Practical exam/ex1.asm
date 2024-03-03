bits 32

global start

extern exit, printf, scanf, gets, fopen, fprintf, fclose
import scanf msvcrt.dll
import printf msvcrt.dll
import exit msvcrt.dll
import gets msvcrt.dll
import fopen msvcrt.dll
import fprintf msvcrt.dll
import fclose msvcrt.dll

segment data use32 class=data
    n_str db "n = ", 0
    int_format db "%d", 0
    str_format db "%s", 13, 10, 0
    n dd 0
    first_letter db 0
    len_message dd 0
    space db " ", 0
    file_name db "words.txt", 0
    access_mode db "w", 0
    file_descriptor dd -1
    message resb 101
    result resb 101
    
segment code use32 class=code
    start: 
        push dword access_mode     
        push dword file_name
        call [fopen]
        add esp, 4*2
        mov [file_descriptor], eax ; store the file descriptor retured by eax
        
        push message
        ;push str_format
        call [gets]
        add esp, 4
        
        push n_str
        call [printf]
        add esp, 4
        
        push dword n
        push int_format
        call [scanf]
        add esp, 4*2
            
        mov esi, message
        mov edi, result ; cuvantul curent + 0 la final
        cld
        reading:
            lodsb ; AL = [ESI], ESI++
            mov bl, [len_message]
            cmp bl, 0
            jne continue
                mov [first_letter], al
            continue:
            cmp al, "#"
            je end_reading
                cmp al, " " ; vedem daca s-a terminat cuvantul
                je check_word
                    stosb ; [EDI] = AL, EDI++
                    inc dword[len_message]
                    jmp reading
                check_word:
                    cmp dword[len_message], 0
                    je reading
                        mov bh, [edi - 1]
                        cmp bh, [first_letter]
                        jne reset
                            mov cl, [len_message]
                            cmp cl, [n]
                            jl reset
                                push edi ; MEMORAM EDI CA SA STIM UNDE AM RAMAS
                                sub edi, [len_message]
                                
                                push dword edi
                                push dword [file_descriptor]
                                call [fprintf]
                                add esp, 4*2
                                
                                push space
                                push dword [file_descriptor]
                                call [fprintf]
                                add esp, 4*2
                                
                                ; push edi
                                ; push str_format
                                ; call [printf]
                                ; add esp, 4*2
                                pop edi
                                inc edi ; lasam 0 dupa fiecare cuvant
                                
                    reset:
                        mov dword[len_message], 0 ; resetam lungimea
                        jmp reading
        
        end_reading:
            cmp dword[len_message], 0
            je the_end
                push edi
                sub edi, [len_message]
                
                push dword edi
                push dword [file_descriptor]
                call [fprintf]
                add esp, 4*2
                
                push space
                push dword [file_descriptor]
                call [fprintf]
                add esp, 4*2
                
                ; push edi
                ; push str_format
                ; call [printf]
                ; add esp, 4*2
                
                pop edi
        the_end:
            push dword [file_descriptor]
            call [fclose]
            add esp, 4
        
        push dword 0
        call [exit]