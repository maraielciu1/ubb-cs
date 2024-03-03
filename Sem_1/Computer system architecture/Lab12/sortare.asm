bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    sir db 1,7,2,3,19,0,4,10
    numar db 0
; our code starts here
segment code use32 class=code
    start:
        mov esi, sir
        mov ecx, 8
        primul_for:
            lodsb ; al = current element esi+1
            mov edi,esi
            push ecx
            
            doi_for:
                mov bl, [edi]
                cmp al,bl
                jle mai_mic
                    mov [edi],al
                    mov [esi-1], bl
                    mov al,bl
                mai_mic:
                inc edi
            loop doi_for
            pop ecx
        loop primul_for
            
    
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
