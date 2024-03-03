bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit,printf,scanf,fopen,fclose,fprintf  ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions
import printf msvcrt.dll
import scanf msvcrt.dll
import fopen msvcrt.dll
import fclose msvcrt.dll
import fprintf msvcrt.dll

;extern find_max 
; Read a string of signed numbers in base 10 from the keyboard. Determine the maximum value of the string and write it in the file max.txt (it will be created) in base 16.
; our data is declared here (the variables needed by our program)
segment data use32=data
    file_name db "max.txt",0
    access_mode db "w", 0
    file_descriptor dd -1       
    len dd 0
    input_len db "# of numbers: ",13,10,0
    numbers dd 0
    input_text db "Type the string of numbers: ",13,10,0
    format db "%d",0
    result dd 0
    sir_inv db "%x", 0
    
; our code starts here
segment code use32=code
   find_max:
        ; esp - ret addr
        ; esp + 4 - number
        ; esp + 8 - result
        mov eax, [esp + 8]
        mov ebx, [esp + 4]
        cmp eax, ebx
        jg not_change
        mov eax, [esp + 4]
    not_change:
        ret 8
    start:
        ; read the # of elements
        push input_len
        call [printf]
        add esp, 4

        push len
        push format 
        call [scanf]
        add esp, 4*2

        push input_text
        call [printf]
        add esp, 4

        mov ecx, [len]

        cmp ecx, 0
        jne continue
            jmp final
        continue:
        ; the initial value of the max is the first number
        dec ecx
        push ecx
        
        push numbers
        push format
        call [scanf]
        
        add esp, 4*2
        mov ebx, [numbers]
        mov [result], ebx
        
        pop ecx
        
        cmp ecx, 0
        jne continue1
            jmp final1
        continue1:
    
    citire:
        push ecx
        push numbers
        push format
        call [scanf]
        add esp, 4*2

        push dword [result]
        push dword [numbers]
        call find_max

        mov [result], eax
        pop ecx
        loop citire
        
    final1:
        
        push dword access_mode     
        push dword file_name
        call [fopen]
        add esp, 4*2            

        mov [file_descriptor], eax  
        cmp eax, 0
        je final
        
        push dword [result]
        push dword sir_inv
        push dword [file_descriptor]
        call [fprintf]
        add esp, 4*3

        push dword [file_descriptor]
        call [fclose]
        add esp, 4


    final:
        ; exit(0)
        push dword 0      ; push the parameter for exit onto the stack
        call [exit]       ; call exit to terminate the program