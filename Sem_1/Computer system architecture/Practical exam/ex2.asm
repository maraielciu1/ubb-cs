bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit,gets,printf,fprintf,fopen,fclose              ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions
import gets msvcrt.dll
import printf msvcrt.dll
import fopen msvcrt.dll
import fclose msvcrt.dll
import fprintf msvcrt.dll

;1    -4 +   8   / 1  - 5  *  9
; 20 de spatii
; 9 cifre

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    file_descriptor dd -1
    access_mode db "w",0
    file_name db "textulmeu.txt",0
    sir resb 101
    spatii dd 0
    text_rezultate db 13,10,"nr total de spatii este %d, nr total de cifre este %d, iar sirul schimbat este %s",0
    numere dd 0
    rezultat_sir resb 101
    rezultat_no_space resb 101
    rezultat db 0
    format_nr db '%d',0
    newline db 10,0
; our code starts here
segment code use32 class=code
    start:
        ; citire sir
        mov ebx,0
        push sir
        call [gets]
        add esp,4
        
        push sir
        call [printf]
        add esp,4
        
        mov esi, sir
        mov edi, rezultat_sir
        
        parcurgere:
            lodsb ; al current byte
            cmp al, 0
            je end_parc
            cmp al, '0'
            jb not_digit
                cmp al, '9'
                ja not_digit
                inc dword [numere]
                mov bl,al
                sub bl, '0'
                add byte [rezultat],bl
            not_digit:
            cmp al, ' '
            jne not_space
                inc dword [spatii]
            not_space:
            cmp al, '-'
            jne not_change1
                mov al, '+'
            not_change1:
            cmp al, '*'
            jne not_change2
                mov al, '+'
            not_change2:
            cmp al, '/'
            jne not_change3
                mov al, '+'
            not_change3:
            stosb
            
        jmp parcurgere
        end_parc:
        mov edi,0
        
        push rezultat_sir
        push dword [numere]
        push dword [spatii]
        push text_rezultate
        call [printf]
        add esp, 4*4
        
        
        push dword access_mode     
        push dword file_name
        call [fopen]
        add esp, 4*2
        mov [file_descriptor], eax  ; store the file descriptor returned by fopen
        
        ; check if fopen() has successfully created the file (EAX != 0)
        cmp eax, 0
        je final1
        
        mov esi, rezultat_sir
        mov edi, rezultat_no_space
        parcurgere1:
            lodsb
            cmp al, 0
            je final
            cmp al, ' '
            je skip
            stosb
            skip:
        jmp parcurgere1
        final:
        mov edi,0
        
        push dword rezultat_no_space
        push dword [file_descriptor]
        call [fprintf]
        add esp, 4*2
        
        push dword newline
        push dword [file_descriptor]
        call [fprintf]
        add esp, 4*2
         
        
        mov eax, 0
        mov al, [rezultat]
        
        push eax
        push dword format_nr
        push dword [file_descriptor]
        call [fprintf]
        add esp, 4*3

        push dword [file_descriptor]
        call [fclose]
        add esp, 4

        final1:
        
    
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
