bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions


; Two byte strings A and B are given. Obtain the string R by concatenating the elements of B in reverse order and the elements of ;A in reverse order.
;Example:
;A: 2, 1, -3, 0
;B: 4, 5, 7, 6, 2, 1
;R: 1, 2, 6, 7, 5, 4, 0, -3, 1, 2    
                    
; our data is declared here (the variables needed by our program)
segment data use32 class=data
    a db 2,1,-3,0
    len_a equ $-a 
    b db 4,5,7,6,2,1
    len_b equ $-b
    r times (len_a+len_b) db 0

; our code starts here
segment code use32 class=code
    start:
        mov ecx,len_b 
        mov esi, len_b-1
        mov edi,0
        jecxz end
            repeta:
                mov al, [b+esi]
                mov [r+edi], al
                dec esi
                inc edi
             loop repeta
        end:
        mov ecx,len_a
        mov esi, len_a-1
        jecxz end1
            repeta1:
                mov al, [a+esi]
                mov [r+edi], al
                dec esi
                inc edi
             loop repeta1
        end1:
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
