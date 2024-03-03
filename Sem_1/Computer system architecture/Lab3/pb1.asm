bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    ; a - byte, b - word, c - double word, d - qword - Unsigned representation
    a db 21h
    b dw 45ABh
    c dd 02DC45A1h
    d dq 0018EFC3AC64112Bh

; our code starts here
segment code use32 class=code
    start:
        ; (d+d-b)+(c-a)+d = 0031 DF87 58C7 DCAB + 2DC 4580 + 0018EFC3AC64112B = 4A CF4B 0808 3356 edx:eax = final res
        
        ; Unsigned
        mov eax, [d] ; the lower part of d goes into eax
        mov edx, [d+4] ; the higher part of d goes into edx
        
        add eax, [d]
        adc edx, [d+4] ; edx:eax = d+d
        
 
        mov ebx,0
        mov bx, [b] ; ebx = b
        
        sub eax, ebx ; d+d-b
        ; edx:eax = 10 A9 34 D6 BB C2 22 00 - 
              ;ebx =            CC 35 77 D2
              ;     
        sbb edx, 0 ; the high part of d+d - cf
        ; edx:eax = (d+d-b)
        
        mov ecx, 0
        mov cl, [a] ; ecx = a
        mov ebx, [c]
        sub ebx,ecx ; ebx = c-a
        
        add eax, ebx
        adc edx, 0
        
        add eax, [d]
        adc edx, [d+4] ; edx:eax = final res
        
        
        
        
        
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
