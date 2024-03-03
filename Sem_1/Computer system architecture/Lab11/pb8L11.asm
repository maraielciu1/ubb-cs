bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit   ,fopen,fclose,fscanf ,perror,printf
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
import fopen msvcrt.dll                         ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions
import fscanf msvcrt.dll
import fclose msvcrt.dll
import perror msvcrt.dll
import printf msvcrt.dll
;Se citesc din fisierul numere.txt mai multe numere (pare si impare). 
;Sa se creeze 2 siruri rezultat N si P astfel: N - nr negative si P - nr pozitive
;Afisati cele 2 siruri rezultate pe ecran.
; our data is declared here (the variables needed by our program)
segment data use32 class=data
    
    file_name db "numere.txt",0
    format db "%d",0
    file_desc dd -1
    nr dd 0
    lp db 0
    ln db 0
    mode db "r",0
    spos times 20 dd 0
    sneg times 20 dd 0
    msg1 db "numere negative:",13,10,0
    msg2 db 13,10,"numere positive:",13,10,0
    msg db "error",0
; our code starts here

segment code use32 class=code
    is_positive:
        mov eax,[esp+4]
        shl eax,1
        jc neg;jmp if cf=1 ->neg
            mov eax,1
            jmp sf
        
        neg:
        mov eax,0
        sf:
      ret 4
     
    afisare:
        ;esp -val retur
        ;esp+4 sir
        ;esp+8 lungime
        
        mov ecx,[esp+8]
        mov esi,[esp+4]
        
        repeta:
            push ecx
            lodsd
            push eax
            push dword format
            call[printf]
            add esp,4*2
            
            pop ecx
         loop repeta
         ret 8 
    
    start:
        
        push dword mode
        push dword file_name
        call [fopen]
        add esp,4*2
        
        cmp eax,0
        je end 
        mov [file_desc],eax
        
        push nr 
        push format
        push dword[file_desc]
        call [fscanf]
        add esp,4*3
        
        mov ecx,[nr]
        mov esi,spos
        mov edi,sneg
        citire:
        
            push ecx 
            
            push nr 
            push format
            push dword[file_desc]
            call [fscanf]
            add esp,4*3
            
            push dword[nr]
            call is_positive
            
            cmp eax,0
            jne positive
                ;e negativ 
                mov eax,[nr]
                stosd ;edi<-eax edi+4
                inc byte[ln]
                jmp endif
            
            positive:
                inc byte[lp]
                mov eax,[nr]
                mov [esi],eax
                add esi,4
            endif:
            pop ecx 
        loop citire
            
        
        
        push dword[file_desc]
        call[fclose]
        add esp,4
        
        
        push dword msg1
        call[printf]
        add esp,4
        
        mov eax,0
        mov al,[ln]
        push eax
        push sneg
        call afisare
        
        push dword msg2
        call[printf]
        add esp,4
        
        mov eax,0
        mov al,[lp]
        push eax
        push spos
        call afisare
        
        jmp no_error
        end:
        push dword msg
        call [perror]
        add esp,4
        no_error:
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
