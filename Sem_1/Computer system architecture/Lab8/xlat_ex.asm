bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit, scanf, printf              ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions
import scanf msvcrt.dll
import printf msvcrt.dll
; our data is declared here (the variables needed by our program)
segment data use32 class=data
    tabhexa db '0123456789ABCDEF'
    numar dd 0
    numar_final times 10 db 0
    sir_final times 10 db 0
    string1 db "introduceti numarul",13,10,0
    rformat db "%d",0
    pformat db "rezultatul e %h",13,10,0
    

; our code starts here
segment code use32 class=code
    start:
        ;citim nr
        push string1
        call [printf]
        add esp,4
        
        push dword numar
        push dword rformat
        call [ scanf]
        add esp,4*2
        mov eax,[numar]
        
        mov esi,numar_final
        mov eax, [numar]
        while_label:
           mov edx,0
           mov ebx,16
           div ebx ; eax = nr/16      edx = nr%16
           mov ecx,eax
           mov ebx, tabhexa
           mov al,dl
           xlat ; al -> noua valoare (coresp in hexa)
           mov [esi],al
           inc esi
           cmp ecx,0
           je afara
           mov eax,ecx
        jmp while_label
        afara:
        
        mov ecx, esi
        sub ecx, numar_final
        dec esi
        mov edi,sir_final
        afisare:
            mov al, [esi]
            mov [edi],al
            inc edi
            dec esi
        loop afisare
        
        ; printf(sir_final)
        push sir_final
        call [printf]
        add esp,4
        
        
        
        
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
