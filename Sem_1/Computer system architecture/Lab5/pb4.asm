bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

;Given a byte string S of length l, obtain the string D of length l-1 as D(i) = S(i) * S(i+1) (each element of D is the product of two consecutive elements of S).
;Example:
;S: 1, 2, 3, 4
;D: 2, 6, 12
                          
; our data is declared here (the variables needed by our program)
segment data use32 class=data
    s db 1,2,3,4
    len equ $-s
    d times len dw 0

; our code starts here
segment code use32 class=code
    start:
        mov esi,0
        mov ecx, len-1
        jecxz end
            repeta:
                mov al, [s+esi]
                mov bl, [s+esi+1]
                mul bl ; ax = al*bl 
                mov [d+esi], ax
                inc esi
            loop repeta
        end:
    
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
