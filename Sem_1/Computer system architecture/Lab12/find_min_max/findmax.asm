bits 32

global _findmax

; the linker may use the public data segment for external datta
segment data public data use32

; the code written in assembly language resides in a public segment, that may be shared with external code
segment code public code use32
; int finmax(int, int)
; cdecl call convention
_findmax:
    ; create a stack frame
    push ebp
    mov ebp, esp
    
    ; retreive the function's arguments from the stack
    ; [ebp+4] contains the return value 
    ; [ebp] contains the ebp value for the caller
    mov eax, [ebp + 8]        ; eax <- max
    
    mov ebx, [ebp + 12]        ; ebx <- nr
    
    cmp eax, ebx            ; compute the max
                            ; the return value of the function should be in EAX
    jg skip
    mov eax,ebx
    skip:
    ; restore the stack frame
    mov esp, ebp
    pop ebp

    ret
    ; cdecl call convention - the caller will remove the parameters from the stack