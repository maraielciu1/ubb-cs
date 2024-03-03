bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit,printf,scanf,fopen,fclose,fprintf ,fread              ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

 import printf msvcrt.dll
import scanf msvcrt.dll
import fopen msvcrt.dll
import fclose msvcrt.dll
import fprintf msvcrt.dll 
import fread msvcrt.dll
; Sa se citeasca de la tastatura numele fisierului de output, un caracter special c si o cifra n (n < 20). Scrieti continutul primului fisier in fiserul de output, dar inlocuiti octetii de pe pozitiile multiplu n cu caracterul special c.

;A special character c, a number n (n<20) and the name of the output file are read from the keyboard. Write the content of the input file in output ;file, but replace the bytes from the positions that are multiple of n with the special character c.
; our data is declared here (the variables needed by our program)
segment data use32 class=data
    file_mesaj db "numele fisierului de output: ",0
    output_file resb 12
    format db "%s",0
    input_file db "ana.txt",0
    file_descriptor_in dd -1
    file_descriptor_out dd -1
    access_mode_in db "r",0
    access_mode_out db "w",0
    mesaj db "eroare la deschidere",0
    n_mesaj db "n=",0
    c_mesaj db "c=",0
    formatn db "%d",0
    formatc db " %c",0
    text resb 101
    caracter db 0
    numar dd 0
    lungime db 0
; our code starts here
segment code use32 class=code
    start:
        push file_mesaj
        call [printf]
        add esp,4
        
        push output_file
        push format
        call [scanf]
        add esp,4*2
        
        push output_file
        call [printf]
        add esp,4
        
        push c_mesaj
        call [printf]
        add esp,4
        
        push caracter
        push formatc
        call [scanf]
        add esp, 4*2
        
        push n_mesaj
        call [printf]
        add esp,4
        
        push numar
        push formatn
        call [scanf]
        add esp, 4*2
    
        
        push access_mode_in
        push input_file
        call [fopen]
        add esp, 4*2
        
        cmp eax,0
        je ends
            mov [file_descriptor_in],eax
            
            push dword [file_descriptor_in]
            push dword 100
            push dword 1
            push dword text        
            call [fread]
            add esp, 4*4
            
            mov esi, text
            parcurgere:
                cmp byte [esi],0
                je no_parcurgere
                    inc byte [lungime]
                    mov eax,0
                    mov edx,0
                    mov al,[lungime]
                    div dword[numar]
                    cmp edx,0
                    jne skip
                        mov al, [caracter]
                        mov byte [esi],al
                    skip:
                    inc esi
            jmp parcurgere
            no_parcurgere:    

            push access_mode_out
            push output_file
            call [fopen]
            add esp, 4*2
            
            cmp eax,0
            je ends
                mov [file_descriptor_out],eax
                
                push dword text
                push dword[file_descriptor_out]
                call [fprintf]
                add esp, 4*2
                
                push dword [file_descriptor_out]
                call [fclose]
                add esp,4
                
            push dword text
            push dword format
            call [printf]
            add esp, 4*3
            
            jmp end1
        
        
        ends:
        push mesaj
        call [printf]
        add esp,4
        end1:
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
