bits 32

global start

; declare external functions needed by our program
extern exit, fscanf, fopen, fclose, printf, scanf
import exit msvcrt.dll
import fopen msvcrt.dll
import fscanf msvcrt.dll
import fclose msvcrt.dll
import scanf msvcrt.dll
import printf msvcrt.dll

segment data use32 class=data
    format_s db '%s', 0
    format_s_spatiu db '%s ', 13, 10, 0
    format_d db "%d", 0
    format_print db "%d ", 0
    str_format db "n = %d", 0
    file times 100 db -1
    n dd 0
    access_mode db 'r', 0
    file_desc dd -1
    message db "file not opened", 0
    mess1 db 13, 10, "numbers were read successfully", 0
    num  dd 0
    doi db 2
    numar resb 100
    cnt db 0
    
segment code use32 class=code
    start:
        ; scanf(format, variable)
        push dword file
        push dword format_s
        call [scanf]
        add esp, 4*2
        
        push dword n
        push dword format_d
        call [scanf]
        add esp, 4*2
        
        ; push file
        ; call [printf]
        ; add esp, 4
        
        ; fopen(nume, access_mode)
        push dword access_mode
        push dword file
        call [fopen]
        add esp, 4*2
        
        cmp eax, 0
        je ending_error
            ; push dword [n]
            ; push str_format
            ; call [printf]
            ; add esp, 4*2
            
            mov [file_desc], eax
            
            mov ecx, [n]
            cmp ecx, 0
            je stop_read
            
            read:
                jecxz stop_read
                push ecx
                
                push numar 
                push format_d
                push dword[file_desc]
                call [fscanf]
                add esp,4*3
                pop ecx
                
                mov esi, numar
                    push ecx
                    lodsd
                    cmp eax, 0
                    je stop_read
                    
                    ; see conditions
                    
                    ; clear high part of eax
                    mov dword[num], eax
                    div byte[doi]
                    cmp ah, 1
                    je odd_num
                        mov ax, word[num]
                        mov bl, [n]
                        div bl
                        cmp ah, 0
                        jne odd_num
                            ; print num
                            push dword[num]
                            push dword format_print
                            call [printf]
                            add esp,4*2
                odd_num:  
                pop ecx
            loop read
            
            stop_read: 
            
        push mess1
        call [printf]
        add esp, 4
        
        ;fclose(descriptor)
        push dword[file_desc]
        call [fclose]
        add esp, 4
        jmp ending
        
        ending_error:
            push message
            call [printf]
            add esp, 4
        
        ending:
        
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program