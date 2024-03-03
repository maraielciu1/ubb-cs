bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit, printf, scanf              ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions
import printf msvcrt.dll
import scanf msvcrt.dll
; our data is declared here (the variables needed by our program)
segment data use32 class=data
    sir_initial resd 10
    sir_final resb 10
    n dd -1
    numar dd 0
    format db "%d",0
    zece dd 10
    suma db 0
    cat dd 0
    two dd 2
; our code starts here
segment code use32 class=code
    start:
        push dword n
        push format
        call [scanf]
        add esp, 4*2
        
        mov ecx, [n]
        ;jecxz end
            mov edi, sir_initial
            citire:
                push ecx 
                
                push dword numar
                push format
                call [scanf]
                add esp, 4*2
                
                mov eax, [numar]
                stosd
                
                pop ecx
            loop citire  
            
            mov esi, sir_initial
            mov edi, sir_final
            mov ecx, [n]
            parcurgere:
                lodsd ; eax = current double word
                mov byte[suma],0
                cifre:
                    mov edx,0
                    div dword[zece]
                    
                    mov [cat], eax
                    mov eax, edx
                    mov edx,0
                    div dword[two]
                    cmp edx,1
                    je skip
                        add byte [suma],al
                        add byte [suma],al
                    skip:
                    
                    mov eax, [cat]
                    cmp eax,0
                jne cifre
                
                mov al, [suma]
                stosb 
            loop parcurgere
            
            mov esi, sir_final
            mov ecx, [n]
            afisare:
                push ecx
                mov eax,0
                lodsb 
                push eax
                push format
                call [printf]
                add esp, 4*2
                ;inc esi
                pop ecx
            loop afisare
            
        
        ;end:
    
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
