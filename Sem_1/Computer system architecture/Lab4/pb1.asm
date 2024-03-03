bits 32 ; assembling for the 32 bits architecture

;Given the word A and the byte B, compute the doubleword C as follows:
;the bits 0-3 of C are the same as the bits 6-9 of A
;the bits 4-5 of C have the value 1
;the bits 6-7 of C are the same as the bits 1-2 of B
;the bits 8-23 of C are the same as the bits of A
;the bits 24-31 of C are the same as the bits of B

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    a dw 1001010010111011b
    b db 11100101b
    c dd 0 ; 11100101 10010100 10111011 10110010

; our code starts here
segment code use32 class=code
    start:
        
        mov ebx,0
        ;the bits 0-3 of C are the same as the bits 6-9 of A
        mov ax,[a]
        and ax, 0000001111000000b
        mov cl, 6
        ror ax,cl 
        or bx,ax ; 0000000000 00 0010 b
        
        ;the bits 4-5 of C have the value 1
        or bx, 0000000000110000b ; 0000000000 11 0010 b
        
        ;the bits 6-7 of C are the same as the bits 1-2 of B
        mov al, [b] ; al = 11100101b
        and al, 00000110b ; al = 00000 10 0b
        mov cl,5
        rol al,cl ; al = 10 000000b
        mov ah, 0
        or bx, ax ; 00000000 10 11 0010 b
        
        ;the bits 8-23 of C are the same as the bits of A
        ; ebx = 00000000 00000000 00000000 10 11 0010 
        mov eax, 0
        mov ax, [a] ; eax = 00000000 00000000 10010100 10111011
        mov cl,8
        rol eax,cl ; eax = 00000000 10010100 10111011 00000000
        or ebx,eax ; ebx = 00000000 10010100 10111011 10110010
        
        ;the bits 24-31 of C are the same as the bits of B
        mov eax,0
        mov al,[b] ; eax = 00000000 00000000 00000000 11100101 
        mov cl,24
        rol eax,cl ; eax = 11100101 00000000 00000000 00000000
        or ebx,eax ; ebx = 11100101 10010100 10111011 10110010
        ; ebx = E594 BBB2 
        
        
        
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
