bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit,printf,scanf,fopen,fclose,fprintf              ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions
import printf msvcrt.dll
import scanf msvcrt.dll
import fopen msvcrt.dll
import fclose msvcrt.dll
import fprintf msvcrt.dll
;Se citește de la tastatură un număr N și apoi N numere.
;Să se calculeze suma numerelor pare introduse, să se calculeze suma numerelor impare introduse, apoi diferența dintre cele două sume.
;Să se afișeze în fișierul output.txt cele 3 rezultate, în baza 16, pe linii separate.
; our data is declared here (the variables needed by our program)
segment data use32 class=data
    filename db "output.txt",0
    access_mode db "w",0
    file_descriptor dd -1
    suma_par dd 0
    suma_impar dd 0
    diferenta dd 0
    numar dd 0
    mesaj_n db "N=",0
    n dd 0
    newline db 10,0
    format_read db "%d",0
    format_write db "%x ",0
    doi dd 2
; our code starts here
segment code use32 class=code
    start:
        push mesaj_n
        call [printf]
        add esp, 4
        
        push dword n
        push format_read
        call [scanf]
        add esp, 4*2
        
        mov ecx, [n]
        citire:
            push ecx
            
            push numar
            push format_read
            call [scanf]
            add esp, 4*2
            
            mov eax, [numar]
            mov edx,0
            div dword[doi]
            cmp edx, 1
            mov eax, [numar]
            je impar
                add dword [suma_par], eax
                jmp next
            impar:
                add dword [suma_impar], eax
            next:
            
            pop ecx
        loop citire
        
        mov eax, [suma_par]
        sub eax, [suma_impar]
        mov [diferenta], eax
        
        push access_mode
        push filename
        call [fopen]
        add esp, 4*2
        
        mov [file_descriptor],eax
        cmp eax, 0
        je end1
            push dword [suma_par]
            push format_write
            push dword [file_descriptor]
            call [fprintf]
            add esp, 4*3
        
            push dword newline
            push dword [file_descriptor]
            call [fprintf]
            add esp, 4*2
            
            push dword [suma_impar]
            push format_write
            push dword [file_descriptor]
            call [fprintf]
            add esp, 4*3
            
            push dword newline
            push dword [file_descriptor]
            call [fprintf]
            add esp, 4*2
            
            push dword [diferenta]
            push format_write
            push dword [file_descriptor]
            call [fprintf]
            add esp, 4*3
            
        push dword [file_descriptor]
        call [fclose]
        add esp,4
        end1:
        
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
