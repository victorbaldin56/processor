start:
in ; input
;out
pop rax
;push rax
;out
;jmp rax + -5

in
pop rbx
;push rbx
;out

in
pop rcx
;push rbx
;out

push rbx
;out
push rbx
;out
mul
;out

push rax
;out
push rcx
mul
;out

push 4
mul

sub

pop [rax + 5]
;out
push [rax + 5]
;sqrt
out

jmp start:

hlt
