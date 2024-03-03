bits 32
global find_max
segment code use32 class=code
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