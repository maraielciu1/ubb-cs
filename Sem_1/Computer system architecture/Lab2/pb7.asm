bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    ; ...
    a db 2
    b db 5
    c db 1
    d dw 10

; our code starts here
segment code use32 class=code
    start:
        ; 3*[20*(b-a+2)-10*c]+2*(d-3) = 270+14 = 284
        mov ah, [b]
        sub ah, [a]
        add ah, 2
        
        mov bh, ah
        mov ax, 20
        mul bh
        
        mov bx, ax
        mov ax,10
        mov ch, [c]
        mul ch
        
        sub bx,ax
        mov ax,bx
        mov ch, 3
        mov dx,0
        mul ch
        mov bx,ax
        
        mov cx, [d]
        sub cx, 3
        mov dl, 2
        mov ax,cx
        mul dl
        
        add bx, ax
        
        
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
