bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
  ;  a-doubleword; b,c,d-byte; x-qword
    a dd 10
    b db 5
    c db 4
    d db 2
    x dq 43

; our code starts here
segment code use32 class=code
    start:
    ; UNSIGNED
        ; a-(7+x)/(b*b-c/d+2) = 10 - 50/(25-2+2) = 8
        mov al,[b]
        mul byte [b] ; ax = b*b
        
        mov bx,ax ; bx = b*b
        
        mov ax,0
        mov al,[c] ; ax = c
        div byte [d] ; al = c/d
        mov ah,0 ; ax = c/d 
        mov cx, ax
        mov eax,0
        mov eax,cx ; eax = c/d
        
        mov cx,bx
        mov ebx,0
        mov ebx,cx
        sub ebx,eax ; bx=b*b-c/d 
        add ebx,2 ; bx=b*b-c/d+2
        
        mov eax, [x]
        mov edx, [x+4]
        add eax, 7
        adc edx,0 ; edx:eax = 7+x 
        
        div ebx ; eax=(7+x)/(b*b-c/d+2)
        
        mov ebx,[a]
        sub ebx,eax ; ebx=a-(7+x)/(b*b-c/d+2) 
        
        
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
