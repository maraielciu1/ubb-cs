bits 32 ; assembling for the 32 bits architecture
 
; declare the EntryPoint (a label defining the very first instruction of the program)
global start        
 
; declare external functions needed by our program
extern exit, fopen, fread, fwrite, fclose, fprintf               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll
import fopen msvcrt.dll
import fread msvcrt.dll
import fwrite msvcrt.dll 
import fclose msvcrt.dll 
import fprintf msvcrt.dll  ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions
 
; our data is declared here (the variables needed by our program)
 
;A name of the input file is defined in the data segement. Replace all the small letters from the file with their ASCII code. The result will be saved in an output file.
;Example: the text read from the input file: MariaC; the result written in the output file should be: M9711410597C
segment data use32 class=data
    input_file db "ana.txt", 0
    output_file db "output.txt", 0
    access_mode_r db "r", 0
    acces_mode_w db "w", 0
    desc_in dd -1
    desc_out dd -1
    format_ascii db "%d", 0
    buffer db 0
    
; our code starts here
segment code use32 class=code
    start:
        push access_mode_r
        push input_file
        call [fopen]
        add esp, 4*2
        
        cmp eax, 0
        je end
            mov [desc_in], eax
            
            push acces_mode_w
            push output_file
            call [fopen]
            add esp, 4*2
            
            cmp eax, 0
            je error_writing
                mov [desc_out], eax
            
            read:
                ; fread(buffer, size, count, fd)
                push dword[desc_in]
                push dword 1
                push dword 1
                push buffer
                call [fread]
                add esp, 4*4
                
                cmp eax, 0
                je finish_reading
                    
                    cmp byte[buffer], 'a'
                    jb not_lower
                        cmp byte[buffer], 'z'
                        ja not_lower
                           ;fprintf(FILE *fptr, const char *str, ...)
                           ;fprintf(desc_out, format_ascii, buffer)
                           mov eax, 0
                           mov al, [buffer]
                          
                           push eax
                           push format_ascii
                           push dword[desc_out]
                           call [fprintf]
                           add esp, 4*3
                           
                           jmp skip 
                    not_lower:
                    push dword[desc_out]
                    push dword 1
                    push dword 1
                    push buffer
                    call [fwrite]
                    add esp, 4*4
                    
                    skip:
            jmp read
            finish_reading:
            
            push dword[desc_out]
            call [fclose]
            add esp, 4
            
            error_writing:
            
            push dword[desc_in]
            call [fclose]
            add esp, 4
            
        end:
    
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program