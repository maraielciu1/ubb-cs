bits 32 ; assembling for the 32 bits architecture

;Given the doubleword M, compute the doubleword MNew as follows:
;the bits 0-3 a of MNew are the same as the bits 5-8 a of M.
;the bits 4-7 a of MNew have the value 1
;the bits 27-31 a of MNew have the value 0
;the bits 8-26 of MNew are the same as the bits 8-26 a of M.


; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    m dd 10111001000111010011101011010010b
    mnew dd 0 ;00000001 00011101 00111010 11110110 
    ; 011D 3AF6
; our code starts here
segment code use32 class=code
    start:
        
        ;the bits 0-3 a of MNew are the same as the bits 5-8 a of M.
        mov eax, [m] ; eax = 10111001000111010011101 0110 10010
        and eax, 00000000000000000000000111100000b ; eax = 00000000000000000000000 0110 00000
        mov cl, 5
        ror eax,cl ; eax = 00000000 00000000 00000000 0000 0110
        
        ;the bits 4-7 a of MNew have the value 1
        or eax,0000000000000000000000011110000b ; eax = 00000000 00000000 00000000 1111 0110
        
        ;the bits 27-31 a of MNew have the value 0
        and eax, 00000111111111111111111111111111b ; eax = 00000000 00000000 00000000 1111 0110
        
        ;the bits 8-26 of MNew are the same as the bits 8-26 a of M.
        mov ebx,[m] ; ebx = 10111001 00011101 00111010 11010010
        and ebx, 00000111111111111111111100000000b ; ebx = 00000 001 00011101 00111010 00000000 
        or eax,ebx ; eax = 00000 001 00011101 00111010 11110110
        
        mov dword [mnew], eax 
        
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
