bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

                          
                          
;Being given a string of doublewords, build another string of doublewords which will include only the doublewords from the given string which have an even number of bits with the value 1.

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    a dd 11110001010100101101010010100101b, 10110001010100101101010010100101b
    len_a equ ($-a)/4
    b times len_a dd 0
; our code starts here
segment code use32 class=code
    start:
        mov esi,a 
        mov edi,b
        mov ecx, len_a
        jecxz final
        repeta_sir:
            push ecx
            lodsd
            mov ecx, 32
            mov bl,0 ; contor
            jecxz end
            repeta:
         
                shl eax, 1 ; CF = the high bit
                jnc not_add
                    inc bl
                not_add:
            loop repeta
            end:
            shr bl,1
            jc not_copy
                mov eax, [esi-4]
                stosd
            not_copy:
            pop ecx
         loop repeta_sir
         final:
            
        
        
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
