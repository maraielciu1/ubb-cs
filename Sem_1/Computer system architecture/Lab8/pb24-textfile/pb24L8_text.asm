bits 32 

global start        

; declare external functions needed by our program
extern exit, fopen, fclose, fread, fwrite
import exit msvcrt.dll 
import fopen msvcrt.dll 
import fread msvcrt.dll 
import fclose msvcrt.dll 
import fwrite msvcrt.dll

; our data is declared here 
segment data use32 class=data
    nume_fisier db "input.txt", 0   ; numele fisierului care va fi deschis
    mod_acces db "r+", 0             ; modul de deschidere a fisierului
    descriptor_fis dd -1            ; variabila in care vom salva descriptorul fisierului - necesar pentru a putea face referire la fisier
    nr_car_citite dd 0              ; variabila in care vom salva numarul de caractere citit din fisier in etapa curenta
    len equ 100                     ; numarul maxim de elemente citite din fisier intr-o etapa
    buffer resb len                 ; sirul in care se va citi textul din fisier
    
    
; inlocuieste cifrele cu 'C'
; our code starts here
segment code use32 class=code
    start:
        ; apelam fopen pentru a deschide fisierul
        ; functia va returna in EAX descriptorul fisierului sau 0 in caz de eroare
        ; eax = fopen(nume_fisier, mod_acces)
        push dword mod_acces
        push dword nume_fisier
        call [fopen]
        add esp, 4*2
        
        ; verificam daca functia fopen a creat cu succes fisierul (daca EAX != 0)
        cmp eax, 0                  
        je final
        
        mov [descriptor_fis], eax   ; salvam valoarea returnata de fopen in variabila descriptor_fis
        
        ; echivalentul in pseudocod al urmatoarei secvente de cod este:
        ;repeta
        ;   nr_car_citite = fread(buffer, 1, len, descriptor_fis)
        ;   daca nr_car_citite > 0
        ;       ; instructiuni pentru procesarea caracterelor citite in aceasta etapa
        ;pana cand nr_car_citite == 0
        
        bucla:
            ; citim o parte (100 caractere) din textul in fisierul deschis folosind functia fread
            ; eax = fread(buffer, 1, len, descriptor_fis)
            push dword [descriptor_fis]
            push dword len
            push dword 1
            push dword buffer
            call [fread]
            add esp, 4*4
            
            ; eax = numar de caractere / bytes citite
            cmp eax, 0          ; daca numarul de caractere citite este 0, am terminat de parcurs fisierul
            je cleanup

            mov [nr_car_citite], eax        ; salvam numarul de caractere citie
            
            ; instructiunile pentru procesarea caracterelor citite in aceasta etapa incep aici
            mov ecx, 0  ; index pentru parcurgerea buffer-ului

            procesare_caractere:
                cmp ecx, [nr_car_citite]  ; comparam indexul cu numarul de caractere citite
                jge reluare_bucla         ; daca indexul >= numarul de caractere citite, reluam bucla

                ; citim caracterul curent din buffer
                mov al, [buffer + ecx]

                ; verificam daca caracterul este o cifra
                cmp al, '0'
                jb  nu_e_cifra
                cmp al, '9'
                ja  nu_e_cifra

                ; daca am ajuns aici, caracterul este o cifra, il inlocuim cu 'C'
                mov byte [buffer + ecx], 'C'

            nu_e_cifra:
            ; incrementam indexul si reluam procesarea
            inc ecx
            jmp procesare_caractere

        reluare_bucla:
          
        ; reluam bucla pentru a citi alt bloc de caractere
        jmp bucla
        
      cleanup:
        ; apelam functia fclose pentru a inchide fisierul
        ; fclose(descriptor_fis)
        push dword [descriptor_fis]
        call [fclose]
        add esp, 4
        
      final:  
        ; exit(0)
        push    dword 0      
        call    [exit]       