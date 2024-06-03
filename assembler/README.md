# Ассемблер
Упрощенный язык ассемблера для эмулятора процессора https://github.com/victorbaldin56/processor.

## Сборка
После сборки эмулятора достаточно:
```
make
```

## Примеры
В качестве наиболее иллюстративных примеров были выбраны 2 программы: одна вычисляет факториал
рекурсивно, вторая решает квадратное уравнение. Обе читают данные с `stdin`.

<details>
<summary>Факториал</summary>

```
jmp main

Factorial:
    push rax
    push 0
    je end ; condition of the recursion end

    push rax

    push rax
    push 1
    sub
    pop rax

    call Factorial ; recursive call

    push rax
    mul ; fact(n) = n*fact(n - 1)

    pop rax
    ret

end:
    push 1
    pop rax
    ret

main:
    in
    pop rax

    call Factorial

    push rax
    out

    hlt
```

</details>

<details>
<summary>Квадратное уравнение</summary>

```
main:
    in
    pop rax

    in
    pop rbx

    in
    pop rcx

    push rax
    push 0
    je Linear

    call Discr

    push rdx
    push 0

    ja halt ; if D < 0

    push rdx
    sqrt

    pop rdx
    push rdx

    push rbx
    push -1
    mul
    pop rcx
    push rcx

    add

    push 2
    push rax
    mul
    pop rax
    push rax

    div

    out

    push rcx

    push rdx

    sub
    push rax

    div

    out
    jmp halt

; Linear equation case
Linear:
    push rbx
    push 0
    je halt ; if b == 0

    push 0
    push rcx
    sub

    push rbx
    div
    out
    jmp halt

Discr: ; counting D = b^2 - 4*a*c
    push rbx
    push rbx
    mul

    push rax
    push rcx
    mul

    push 4
    mul

    sub
    pop rdx

    ret

halt:
    hlt
```

</details>
