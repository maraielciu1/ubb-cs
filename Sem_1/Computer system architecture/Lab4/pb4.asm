bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

;Given 4 bytes, compute in AX the sum of the integers represented by the bits 4-6 of the 4 bytes.

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    a db 01100111b ; bits 4-6 = 110
    b db 10000000b ; bits 4-6 = 000
    c db 11101111b ; bits 4-6 = 110
    d db 11111111b ; bits 4-6 = 111
    ; AX = 6 + 0 + 6 + 7 = 19 = 0001 0011b 
; our code starts here
segment code use32 class=code
    start:
        mov al, [a] ; al = 01100111
        and al, 01110000b ; al = 0 110 0000 
        mov cl, 4
        shr al,cl ; al = 0000 0110
        mov ah,0; ax = 0000 0110
        
        mov bl,[b] ; bl = 10000000
        and bl, 01110000b ; bl = 0 000 0000 
        mov cl, 4
        shr bl,cl ; bl = 0000 0000
        mov bh,0 ; bx = 0000 0000
        
        add ax,bx 
        
        mov bl,[c] ; bl = 11101111
        and bl, 01110000b ; bl = 0 110 0000 
        mov cl, 4
        shr bl,cl ; bl = 0000 0110
        mov bh,0 ; bx = 0000 0110
        
        add ax, bx
        
        mov bl,[d] ; bl = 11111111
        and bl, 01110000b ; bl = 0 111 0000 
        mov cl, 4
        shr bl,cl ; bl = 0000 0111
        mov bh,0 ; bx = 0000 0111
        
        add ax, bx ; ax = 19bits 32 ; assembling for the 32 bits architecture


        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
