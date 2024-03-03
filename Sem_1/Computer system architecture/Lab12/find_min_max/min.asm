bits 32
global _mini
segment data use32 public data

segment code use32 public code
    _mini:
        ; esp + 4 - ret addr
        ; esp + 8 - buffer
        ; esp + 12 - result
        push ebp
        mov ebp, esp

        mov eax, [ebp + 12]
        mov ebx, [ebp + 8]
        cmp eax, ebx
        jl no_change
            mov eax, [ebp + 8]
        no_change:
            mov esp, ebp
            pop ebp
            ret