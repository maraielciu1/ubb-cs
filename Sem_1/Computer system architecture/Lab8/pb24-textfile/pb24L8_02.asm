bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit , fopen, fclose, fprintf              ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions
import fopen msvcrt.dll
import fclose msvcrt.dll
import fprintf msvcrt.dll
;A file name and a text (defined in data segment) are given. The text contains lowercase letters, uppercase letters, digits and special characters. Replace all digits from the text with character 'C'. Create a file with the given name and write the generated text to file.
; our data is declared here (the variables needed by our program)
segment data use32 class=data
    file_name db "output.txt",0
    text db "abcd124dchbsu7",0
    len_text equ $-text-1
    mod_acces db "w",0
    file_descriptor dd -1
    

; our code starts here
segment code use32 class=code
    start:
    
        mov ecx, len_text
        mov esi,text
        modifica_text:
            cmp byte [esi],'0'
            jl not_digit
            cmp byte[esi],'9'
            jg not_digit
                ; if it is here, it is a digit
                mov byte[esi],'C'
        
            not_digit:
            inc esi
        loop modifica_text
        
        ; call fopen() to create the file
        ; fopen() will return a file descriptor in the EAX or 0 in case of error
        ; eax = fopen(file_name, access_mode)
        push mod_acces
        push file_name
        call [fopen]
        add esp, 4*2
        
        mov [file_descriptor],eax
        cmp eax, 0
        je final
            ; write the text to file using fprintf()
            ; fprintf(file_descriptor,text)
            push dword text
            push dword [file_descriptor]
            call [fprintf]
            add esp, 4*2
        
            ; call fclose() to close the file
            ; fclose(file_descriptor)
            push dword [file_descriptor]
            call [fclose]
            add esp, 4
        
        final:
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
