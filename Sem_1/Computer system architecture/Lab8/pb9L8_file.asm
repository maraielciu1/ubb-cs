bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        
;A text file is given. Read the content of the file, determine the special character with the highest frequency and display the character along with its frequency on the screen. The name of text file is defined in the data segment.
; declare external functions needed by our program
extern exit, fopen,fclose,fwrite,fread               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions
import fopen msvcrt.dll
import fclose msvcrt.dll
import fread msvcrt.dll
import fwrite msvcrt.dll
; our data is declared here (the variables needed by our program)
segment data use32 class=data
    fd_in dd -1
    fd_out dd -1
    content db 0
    resultc db 0
    len_seq dd 0 
    input_file db "file.txt",0
    output_file db "out.txt",0
    in_mode db 'r',0
    out_mode db 'w',0
    wmode db "the character %c appeared %d times"
    
; our code starts here
segment code use32 class=code
    start:
        push in_mode
        push input_file
        call [fopen]
        add esp,4*2
        
        cmp eax,0
        je end
            mov [fd_in],eax
            mov ebx,0 ; ap_max 
            read_loop:
                push dword [fd_in]
                push dword 1
                push dword 1
                push content
                call [fread]
                add esp,4*4
                
                cmp eax,0
                je out_of_loop
            



            push dword [fd_in]
            call [fclose]
            add esp,4
        end:
        
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
