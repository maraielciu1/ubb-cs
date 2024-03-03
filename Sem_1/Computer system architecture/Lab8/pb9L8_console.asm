bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
; Read two numbers a and b (base 10) from the keyboard and calculate: (a+b)/(a-b). The quotient will be stored in a variable 
; called "result" (defined in the data segment). The values are considered in signed representation.
global start        

; declare external functions needed by our program
extern exit, printf, scanf               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions
import printf msvcrt.dll
import scanf msvcrt.dll

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    a dd 0
    b dd 0
    result dd 0
    in_text_a db "a=",0
    in_text_b db 13,10,"b=",0
    rformat db "%d",0
    pformat db 13,10,"the result is %d ",0

; our code starts here
segment code use32 class=code
    start:
        ; read a
        push in_text_a
        call [printf]
        add esp,4
        push dword a 
        push rformat
        call [scanf]
        add esp, 4*2
        
        ; read b
        push in_text_b
        call [printf]
        add esp,4
        push dword b 
        push rformat
        call [scanf]
        add esp, 4*2
        
        ; compute (a+b)/(a-b)
        mov ax, [a]
        add ax, [b]
        cwd
        
        mov bx,[a]
        sub bx,[b]
        
        idiv bx ; ax = (a+b)/(a-b), dx = (a+b)%(a-b) => result=ax
        cwde
        mov [result],eax
        push eax
        push pformat
        call [printf]
        add esp, 4*2
        
        
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
