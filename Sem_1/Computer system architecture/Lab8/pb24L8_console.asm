bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        
;Two numbers a and b are given. Compute the expression value: (a/b)*k, where k is a constant value defined in data segment. Display the expression value (in base 2).
; declare external functions needed by our program
extern exit, printf, scanf               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions
import printf msvcrt.dll
import scanf msvcrt.dll
; our data is declared here (the variables needed by our program)
segment data use32 class=data
    a dd 0
    b dd 0
    k equ 15
    result times 32 db 0
    contor db 0
    sir_inv times 32 db 0
    a_var db "a=",0
    b_var db 13,10,"b=",0
    rformat db "%d",0
    pformat db 13,10,"the result is %s ",13,10,0

; our code starts here
segment code use32 class=code
    start:
        ; read a 
        push a_var
        call [printf]
        add esp,4
      
        push dword a
        push rformat
        call [scanf]
        add esp,4*2
        
        ; read b 
        push b_var
        call [printf]
        add esp,4
      
        push dword b
        push rformat
        call [scanf]
        add esp,4*2
        
        ; compute (a/b)*k
        mov dx,[a+2]
        mov ax,[a]
        div word [b] ; result in ax 
        mov cx, k
        mul cx ; result in dx:ax
        
        push dx
        push ax
        pop eax
        
        mov edi,result
        mov byte [contor],0
        transformare:
            mov edx,0 ; edx:eax = nr
            mov ebx,2 
            div ebx ; edx = nr%2    eax = nr/2
            add dl,'0'
            mov [edi],dl
            inc edi
            inc byte [contor]
            cmp eax,0
            je end_nr
            
        jmp transformare
        end_nr:
        
        mov esi,edi
        dec esi
        mov ecx,0
        mov cl,[contor]
        mov edi,sir_inv
        jecxz end1
        inversare:
            mov al,[esi]
            mov [edi],al
            dec esi
            inc edi
        loop inversare
        
        push sir_inv
        push pformat
        call [printf]
        add esp,4*2
        end1:
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
