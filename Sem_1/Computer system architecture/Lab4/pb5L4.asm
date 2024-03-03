

; Observation: bits are numbered from right to left

bits 32 ;assembling for the 32 bits architecture
; the start label will be the entry point in the program
global  start 

extern  exit ; we inform the assembler that the exit symbol is foreign, i.e. it exists even if we won't be defining it
import  exit msvcrt.dll; exit is a function that ends the process, it is defined in msvcrt.dll
        ; msvcrt.dll contains exit, printf and all the other important C-runtime functions
segment  data use32 class=data ; the data segment where the variables are declared 
    
segment  code use32 class=code ; code segment
start: 

    mov al, 00110011b
    mov cl, 2
    shl al, cl ; → al = 11001100b, CF = 0, al=CCh
    
    mov bl, 01011110b
    mov cl, 2
    shr bl, cl ; → bl = 00010111b, CF = 1, bl=71h
    
    mov dl, 00110011b
    mov cl, 2
    rol dl, cl ; → dl = 11001100b, CF = 0, dl=CCh
    
    mov al, 00111110b
    mov cl, 2
    ror al, cl ; → al = 10001111b, CF = 1, al=8Fh
    

     push dword 0 ;saves on stack the parameter of the function exit
     call [exit] ;function exit is called in order to end the execution of the program	
