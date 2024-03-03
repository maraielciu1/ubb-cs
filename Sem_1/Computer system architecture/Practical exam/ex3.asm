bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit,scanf,printf               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions
import scanf msvcrt.dll
import printf msvcrt.dll
                          
                          
;Se citeste de la tastatura un cuvant si un numar. Daca numarul este par se cere criptarea cuvantului prin adunarea la fiecare caracter a numarului 20. Daca numarul este impar se cere criptarea cuvantului prin adaugarea dupa fiecare vocala a gruparii "p"+vocala.Se cere afisarea cuvantului criptat.

;ex: arici 3 => aparipicipi

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    cuv_message db "cuvantul :",0
    n_message db "n= ", 0
    formats db "%s",0
    format db "%d", 0
    doi db 2
    cuvant resb 101
    n db 0 
    cuv_final resb 101

; our code starts here
segment code use32 class=code
    start:
        push cuv_message
        call [printf]
        add esp, 4
        
        push cuvant
        push formats
        call [scanf]
        add esp, 4*2
        
        push n_message
        call [printf]
        add esp, 4
        
        push n
        push format
        call [scanf]
        add esp, 4*2
        
        mov al, [n]
        mov ah, 0
        div byte [doi]
        cmp ah, 1
        je adaugarea_impar
            mov esi, cuvant
            mov edi, cuv_final
            repeta:
                lodsb 
                cmp al, 0
                je end_repeta
                    add al, 20
                    stosb
            jmp repeta
        adaugarea_impar:
            mov esi, cuvant
            mov edi, cuv_final
            repeta1:
                lodsb 
                cmp al, 0
                je end_repeta
                    stosb
                    cmp al, 'a'
                    je vocala
                    cmp al, 'e'
                    je vocala
                    cmp al, 'i'
                    je vocala
                    cmp al, 'o'
                    je vocala 
                    cmp al, 'u'
                    je vocala
                    jmp not_vocala
                    vocala:
                        mov byte [edi], 'p'
                        inc edi
                        mov [edi],al
                        inc edi
                    not_vocala:
                    jmp repeta1
        
        end_repeta:
        
        push cuv_final
        call [printf]
        add esp,4
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
