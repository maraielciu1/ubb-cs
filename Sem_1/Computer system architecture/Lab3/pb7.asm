bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    ;  a,b-byte; c-word; e-doubleword; x-qword
    a db 4
    b db 2
    c dw 3
    e dd 10
    x dq 5
    f dw 128

; our code starts here
segment code use32 class=code
    start:
        ; UNSIGNED 
        ; (a-b+c*128)/(a+b)+e-x = 386/6 + 10 - 5 = 64+10-5 = 69
        mov ebx, 0
        mov bl,[a]
        sub bl,[b] ; ebx = a-b
        
        mov ax, [c]
        mov dx, 0
        mul word [f] ; dx:ax = c*128
        
        push dx
        push ax
        pop eax
        
        add eax,ebx
        
        mov bx,0
        mov bl,[a]
        add bl, [b] ; bx=a+b 
        
        div bx ; ax = (a-b+c*128)/(a+b) 
        
        mov bx, ax
        mov eax,0
        mov ax,bx ; eax = (a-b+c*128)/(a+b)
        
        add eax, [e] ; eax = (a-b+c*128)/(a+b)+e 
        
        mov edx,0       ; ??????????
        sub eax, [x]
        sbb edx, [x+4] ; edx:eax = (a-b+c*128)/(a+b)+e-x
        
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
