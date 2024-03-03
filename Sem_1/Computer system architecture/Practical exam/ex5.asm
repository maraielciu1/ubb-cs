bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit,gets, printf               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions
import gets msvcrt.dll
import printf msvcrt.dll
                        
; se citeste o propozitie de la tastaura, sa se afiseze cuvintele inversate, iar in cele cu nr impar de litere, sa se inlocuiasca 
; litera din mijloc cu spatiu
; our data is declared here (the variables needed by our program)
segment data use32 class=data
    text resb 101
    rezultat resb 101
    cuvant resb 30
    cuvant_inversat resb 30
    len dd 0
    doi dd 2
    index_rezultat dd 0
    fromat_afisare db "%s ",0
    str_format db "%s",0

; our code starts here
segment code use32 class=code
    invers:
        ; esp adresa return
        ; esp+4 sir
        ; esp+8 len
        
        mov esi, dword[esp+4]
        mov edi, cuvant_inversat
        
        mov ecx, dword[esp+8]
        add esi, dword[esp+8]
        sub esi, dword 1
        inv_loop:
            std
            lodsb
            cld
            stosb
        loop inv_loop
        mov byte [edi],0
        mov eax, dword[len]
        mov edx,0
        div dword[doi]
        cmp edx, 1
        jne skip
            mov byte [cuvant_inversat+eax],' '
        skip:
        
        push dword cuvant_inversat
        push fromat_afisare
        call [printf]
        add esp, 4*2
        
        
     ret 8
    start:
        ; citim textul initial
        push text
        call [gets]
        add esp, 4
        
        ; parcurgen textul cuvant cu cuvant
        mov esi, text
        mov edi, cuvant
        repeta:
            lodsb ; al current byte
            cmp al,0
            je end_repeta
            cmp al, " "
            je new_word
                stosb
                inc dword[len]
                jmp repeta
            new_word:
                mov byte [edi], 0
                ;push edi
                push esi
                ; invers(sir,len) - inverseaza si afiseaza cuvantul
                push dword [len]
                push cuvant
                call invers
                
                pop esi
                ;pop edi
                mov edi, cuvant
                mov dword[len],0
            jmp repeta
                
        end_repeta:
        mov byte [edi], 0
           ; invers(sir,len) - inverseaza si afiseaza cuvantul
           push dword [len]
           push cuvant
           call invers
                
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
