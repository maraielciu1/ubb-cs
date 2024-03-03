; bits 32 ; assembling for the 32 bits architecture

; ; declare the EntryPoint (a label defining the very first instruction of the program)
; global start        

; ; declare external functions needed by our program
; extern exit,gets, printf               ; tell nasm that exit exists even if we won't be defining it
; import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions
; import gets msvcrt.dll
; import printf msvcrt.dll
; ; se citeste o propozitie de la tastatura, sa se afiseze cuvintele scrise invers
; ; our data is declared here (the variables needed by our program)
; segment data use32 class=data
    ; text resb 101
    ; cuvant resb 101
    ; rezultat resb 101
    ; len db 0

; ; our code starts here
; segment code use32 class=code
    ; start:
        ; push text
        ; call [gets]
        ; add esp, 4
        
        ; mov esi, text
        ; mov edi, cuvant
        ; repeta:
            ; lodsb 
            ; cmp al, 0
            ; je end_repeta
            ; cmp al, ' '
            ; je new_word
                ; stosb
                ; inc byte [len]
                ; jmp repeta
            ; new_word:
                ; push edi
                ; mov ecx, [len]
                ; push esi
                ; mov esi, edi
                ; mov edi, rezultat
                ; invert:
                    ; mov al, [esi]
                    ; mov [edi],al
                    ; inc edi
                    ; dec esi 
                ; loop invert
                ; pop esi
                ; pop edi
                ; mov byte [len],0
        ; jmp repeta
        
        ; end_repeta:
        ; push rezultat 
        ; call [printf]
        ; add esp, 4
bits 32 ; assembling for the 32 bits architecture
; declare the EntryPoint (a label defining the very first instruction of the program)
global start        
; declare external functions needed by our program
extern exit,gets, printf               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions
import gets msvcrt.dll
import printf msvcrt.dll
; se citeste o propozitie de la tastatura, sa se afiseze cuvintele scrise invers
; our data is declared here (the variables needed by our program)
segment data use32 class=data
    text resb 101
    cuvant resb 101
    rezultat resb 101
    len dd 0
    index_rezultat dd 0
    str_format db "%s",0
; our code starts here
segment code use32 class=code
    start:
        push text
        call [gets]
        add esp, 4
        mov esi, text
        mov edi, cuvant
        repeta:
            lodsb 
            cmp al, 0
            je end_repeta
            cmp al, " "
            je new_word
                stosb
                inc dword[len]
                jmp repeta
            new_word:
                push edi
                mov ecx, [len]
                push esi
                mov esi, edi
                dec esi
                mov edi, rezultat
                add edi, dword[index_rezultat]
                invert:
                    mov al, [esi]
                    mov [edi],al
                    inc edi
                    dec esi 
                loop invert
                mov dword[edi], " "
                inc edi
                mov eax, dword[len]
                add dword[index_rezultat], eax
                inc dword[index_rezultat]
                pop esi
                pop edi
                mov dword[len],0
        jmp repeta
        end_repeta:
            push edi
                mov ecx, [len]
                push esi
                mov esi, edi
                dec esi
                mov edi, rezultat
                add edi, dword[index_rezultat]
                invert2:
                    mov al, [esi]
                    mov [edi],al
                    inc edi
                    dec esi 
                loop invert2
             pop esi
                pop edi
        push rezultat 
        push str_format
        call [printf]
        add esp, 4*2
        
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
