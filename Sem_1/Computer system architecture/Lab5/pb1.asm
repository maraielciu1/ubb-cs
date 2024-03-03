bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)

;A byte string S of length l is given. Obtain the string D of length l-1 so that the elements of D represent the difference between every two consecutive elements of S.

segment data use32 class=data
    s db 1,2,4,6,10,20,25
    len equ $-s
    d times len db 0

; our code starts here
segment code use32 class=code
    start:
        mov esi,0
        mov ecx, len-1
        jecxz end
            repeat:
                mov al, [s+esi]
                mov bl, [s+esi+1]
                sub bl,al
                mov [d+esi], bl
                inc esi
            loop repeat
        end:
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
