bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

;An array with doublewords containing packed data (4 bytes written as a single doubleword) is given. Write an asm program in order to obtain a new array of doublewords, where each doubleword will be composed by the rule: the sum of the bytes from an odd position will be written on the word from the odd position and the sum of the bytes from an even position will be written on the word from the even position. The bytes are considered to represent signed numbers, thus the extension of the sum on a word will be performed according to the signed arithmetic.

;12 7F 56 78h, 0ABCDABCDh
; 12 + 56 = 68    7F + 78 = F7
; 0068 00F7h, FF56FF9Ah

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    d dd 127F5678h, 0xABCDABCD
    lend equ ($-d)/4
    res resb lend*4
    sum_odd dw 0
    sum_even dw 0

; our code starts here
segment code use32 class=code
    start:
        mov ecx,lend
        mov esi,d
        mov edi,res
        jecxz stop
            repeta:
                mov word [sum_odd],0
                mov word [sum_even],0
                lodsb ; al = least sig byte from the current word, that always has an odd index
                cbw ; ax=al
                add [sum_odd], ax
                lodsb ; al = most sig byte from the current word, that always has an even index
                cbw ; ax=al
                add [sum_even], ax
                
                
                lodsb ; al = least sig byte from the current word, that always has an odd index
                cbw ; ax=al
                add [sum_odd], ax
                lodsb ; al = most sig byte from the current word, that always has an even index
                cbw ; ax=al
                add [sum_even], ax
                
                mov ax,word [sum_odd] 
                stosw
                mov ax,word [sum_even] 
                stosw
            loop repeta  
        stop:
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
