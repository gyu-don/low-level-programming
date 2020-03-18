section .text

exit:
    mov rax, 60
    syscall
    ret

string_length:
    xor rax, rax
.loop:
    cmp byte [rdi+rax], 0
    je .end
    inc rax
    jmp .loop
.end:
    ret

print_string:
    call string_length
    mov rdx, rax
    mov rsi, rdi
    mov rdi, 1
    mov rax, 1
    syscall
    xor rax, rax
    ret


print_char:
    push rdi
    mov rsi, rsp
    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    syscall
    pop rdi
    xor rax, rax
    ret

print_newline:
    xor rax, rax
    ret


print_uint:
    mov rcx, 10
    mov rsi, rsp
    mov rax, rdi
.loop:
    xor rdx, rdx
    div rcx
    add rdx, 0x30
    dec rsi
    mov [rsi], dl
    cmp rax, 0
    jne .loop
    mov rax, rsp
    sub rax, rsi
    mov rdx, rax
    mov rdi, 1
    mov rax, 1
    syscall
    xor rax, rax
    ret


print_int:
    cmp rdi, 0
    jge .L1
    push rdi
    mov rdi, 0x2d
    call print_char
    xor rdi, rdi
    pop rax
    sub rdi, rax
.L1:
    call print_uint
    ret


read_char:
    mov rsi, rsp
    dec rsi
    mov rax, 0
    mov rdi, 0
    mov rdx, 1
    syscall
    cmp rax, 0
    jz .fail
    mov rax, [rsi]
.fail:
    ret

read_word:
    mov rdx, rsi
    mov rsi, rdi
    mov rdi, 0
    mov rax, 0
    syscall
    cmp rax, 0
    jl .err
    cmp rax, rsi
    je .full
    mov byte [rsi+rax], 0
    mov rdx, rax
    mov rax, rsi
    ret
.err:
.full:
    xor rax, rax
    ret

; rdi points to a string
; returns rax: number, rdx : length
parse_uint:
    mov rcx, 10
    xor r8, r8
    xor rax, rax
    xor rsi, rsi
.loop:
    mov sil, [rdi]
    cmp rsi, 0x30
    jb .break
    cmp rsi, 0x39
    ja .break
    sub rsi, 0x30
    mul rcx
    add rax, rsi
    add r8, 1
    add rdi, 1
    jmp .loop
.break:
    mov rdx, r8
    ret

; rdi points to a string
; returns rax: number, rdx : length
parse_int:
    xor rax, rax
    mov al, [rdi]
    cmp rax, 0x2d
    je .neg
    cmp rax, 0x2b
    je .pos
    call parse_uint
    ret
.pos:
    add rdi, 1
    call parse_uint
    add rdx, 1
    ret
.neg:
    add rdi, 1
    call parse_uint
    add rdx, 1
    neg rax
    ret


string_copy:
.loop:
    mov rax, [rdi]
    mov [rsi], rax
    cmp byte [rdi], 0
    je .end
    inc rsi
    inc rdi
    jmp .loop
.end:
    xor rax, rax
    ret

;string_equals:
;.loop:
;    cmp [rdi], [rsi]
;    jne .notsame
;    cmp [rdi], 0
;    jnz .loop
;    mov rax, 1
;    ret
;.notsame:
;    xor rax, rax
;    ret
