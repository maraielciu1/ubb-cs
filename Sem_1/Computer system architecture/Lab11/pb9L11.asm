bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit,printf,scanf               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions
import printf msvcrt.dll
import scanf msvcrt.dll
; our data is declared here (the variables needed by our program)
segment data use32 class=data
    len dd 0
    number dd 0
    input_text db "Number of elements: ",13,10,0
    input_tetx1 db "Give the elements ",13,10,0
    format1 db "%u",0 ; unsigned
    format2 db "%x",0 ; hexa
    format3 db "%d",0 ; signed
    text_uns db " unsigned %u ",0
    text_sign db " signed %d ",0
    sir times 100 dd 0
    contor db 0
; Read from the keyboard a string of numbers, given in the base 16 (a string of characters is read from the keyboard and a string of numbers must be stored in memory). Show the decimal value of the number both as unsigned and signed numbers.
; our code starts here
segment code use32 class=code
    start:
        push input_text
        call [printf]
        add esp,4
        
        push len 
        push format3
        call [scanf]
        add esp,4*2
        
        mov edi,sir
        
        mov ecx,[len]
        jecxz final
        push ecx
        push input_tetx1
        call [printf]
        add esp,4
        pop ecx
        citire:
        push ecx
            push number
            push format2
            call [scanf]
            add esp,4*2
            
            push dword[number]
            push text_uns
            call [printf]
            
            push dword[number]
            push text_sign
            call [printf]
        pop ecx
        loop citire
        
        
        final:
    
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
