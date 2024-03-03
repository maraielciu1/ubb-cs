bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

                          
                          
                          
;A list of doublewords is given. Starting from the low part of the doubleword, obtain the doubleword made of the high even bytes of the low words of each doubleword from the given list. If there are not enough bytes, the remaining bytes of the doubleword will be filled with the byte FFh.
;Example:
;Given the string of doublewords:
; s DD 12345678h, 1A2C3C4Dh, 98FCDD76h, 12783A2Bh
;obtain the doubleword:
;d DD FF3A3C56h


; our data is declared here (the variables needed by our program)
segment data use32 class=data
    s dd 12345678h, 1A2C3C4Dh, 12783A2Bh
    len_s equ ($-s)/4
    d dd 0
    a db 0
    b db 4

; our code starts here
segment code use32 class=code
    start:
        mov esi,s
        mov edi,d
        mov ecx,len_s
        jecxz end
            repeat:
                lodsb
                movsb
                inc byte [a]
                lodsw
            loop repeat
        end:
        
        mov ecx, 0
        mov bl, byte [b]
        sub bl, byte [a]
        mov cl,bl
        jecxz end1
            repeat1:
                mov byte [edi], 0xFF
                inc edi
            loop repeat1
        end1:
    
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
