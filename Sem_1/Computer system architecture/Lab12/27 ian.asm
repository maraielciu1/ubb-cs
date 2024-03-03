bits 32 ; assembling for the 32 bits architecture
; declare the EntryPoint (a label defining the very first instruction of the program)
global start        
; declare external functions needed by our program
extern exit,printf,scanf               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions
import printf msvcrt.dll
import scanf msvcrt.dll
; our data is declared here (the variables needed by our program)
; se citeste n intre 1 si 6, sa se sorteze fiecare byte de pe pozitie divizibila cu n in ordine descrescatoare in functie de nr de biti de 1 in repr
segment data use32 class=data
    n dd 0
    sir_nr dq 1122334455667788h, 99aabbccddeeff11h, 0ff00000000000055h, 0fffeffffffffefffh, 450000ab000000abh, 11113733555577abh
    len dd $-sir_nr
    string times 20 db 0
    sir_bit times 6 db 0
    format_citre db "%d",0
    format_afisare db "%x",0
    lungime dd 0
    element db 0
    len_string dd 0
    nr_bit db 0
    el1 db 0
    el2 db 0
; our code starts here
segment code use32 class=code
    start:
        push dword n
        push format_citre
        call [scanf]
        add esp, 4*2
        cmp dword[n],1
        jl end 
        cmp dword [n], 6
        ja end
            mov esi, sir_nr
            mov edi, string
            mov ecx, [len]
            parcurgere:
                lodsb 
                inc dword[lungime]
                mov [element],al
                mov eax, [lungime]
                mov edx,0
                div dword[n]
                cmp edx,0
                jne skip
                    mov al, [element]
                    stosb
                    inc dword[len_string]
                skip:
            loop parcurgere
            mov esi, string
            mov edi, sir_bit
            mov ecx, [len_string]
            parcurg:
                lodsb ; al = current element
                push ecx
                mov ecx, 8
                mov byte[nr_bit],0
                check:
                    shl al,1
                    jnc nu
                        inc byte[nr_bit]
                    nu:
                loop check
                mov al, [nr_bit]
                stosb
                pop ecx
            loop parcurg
            mov esi, sir_bit
            mov ecx, [len_string]
            primul_for:
                lodsb ; al - current byte
                mov edi,esi
                push ecx
                dec ecx
                jecxz sari_peste
                al_doilea:
                    mov bl, [edi]
                    cmp al,bl ; Daca numarul de biti e mai mare face schimb
                    jl mai_mic
                        ; Interschimbare
                        mov [edi],al
                        mov [esi-1], bl
                        mov al,bl
                        push eax
                        mov edx, edi ; In edx avem locatia elementului nou
                        sub edx, sir_bit ; Scadem sa vedem al catelea element din sirul de biti e
                        mov al, [edx+string] ; AL = elementul de la sirul acela
                        push ecx
                        mov ecx, esi ; IN ecx avem locatia elemntului vechi
                        dec ecx ; Scadem ca sa ajungem la elementul vechi
                        sub ecx, sir_bit ; Scadem ca sa vedem al catelea ii
                        mov bl, [ecx+string] ; BL = elemntul nou
                        mov [edx+string],bl ; Interschimbare
                        mov [ecx+string],al
                        pop ecx
                        pop eax
                    mai_mic:
                    inc edi
                loop al_doilea
                sari_peste:
                pop ecx
            loop primul_for
            mov esi,string
            mov ecx, [len_string]
            afisare:
                push ecx
                mov eax,0
                lodsb 
                push eax
                push format_afisare
                call [printf]
                add esp, 4*2
                pop ecx
            loop afisare
        end:
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program