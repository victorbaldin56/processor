# Процессор
Эмулятор процессора с простым собственным набором инструкций.

## Использование
Получить список доступных опций можно получить, подав аргумент командной
строки `--help`. На входе поддерживает бинарные файлы с байт-кодом.
Файл обязан начинаться с 5-байтовой сигнатуры, поля в следующем порядке:
* Постоянное поле (4 байта): `BAV#`
* Версия эмулятора, которая требуется для выполнения (1 байт)

Для того, чтобы сгенерировать такой файл с байт-кодом из примитивного
языка ассемблера для данного эмулятора, можно воспользоваться вспомогательным
[ассемблером](assembler/README.md) из этого репозитория.

## Сборка
1. Клонировать репозиторий:
```sh
git clone https://github.com/victorbaldin56/processor
```
2. Собрать:
```sh
make
```

## Микроархитектура
Данный эмулятор процессора реализует стековый принцип вычислений, то есть
для любых бинарных арифметических операций справедлив такой порядок действий:
```
push lhs
push rhs
binop       ; Результат на вершине стека
```
Все арифметические инструкции берут операнды (числа с плавающей точкой типа `double` в языке C)
с вершины стека. Для сохранения промежуточных значений предусмотрены 4 регистра: `rax`, `rbx`, `rcx`, `rdx`.
Значения со стека сохраняются в регистр командой `pop`. Из регистра в стек значение можно скопировать
командой `push`, передав ей в качестве операнда регистр.

## Кодировка инструкций
Текущая версия подразумевает 1 байт на инструкцию, при этом значения бит в этом байте следующие:
```
   |||||------------- opcode
   ^^^^^
00000000
^^^
|||------------------ числовой аргумент (imm)
||------------------- регистровый аргумент (r)
|-------------------- зарезервировано
```

На данный момент реализованы следующие инструкции:
| Opcode | Mnemonics | Operand types |
|:-:|:-:|:-:|
| 00   | in  |     - |
| 01   | out |     - |
| 02   | push| imm/r |
| 03   | add |     - |
| 04   | sub |     - |
| 05   | mul |     - |
| 06   | div |     - |
| 07   | sqrt|     - |
| 08   | pop |     r |
| 09   | jmp | imm/r |
| 0a   | jae | imm/r |
| 0b   | jbe | imm/r |
| 0c   | ja  | imm/r |
| 0d   | jb  | imm/r |
| 0e   | je  | imm/r |
| 0f   | call| imm/r |
| 10   | hlt |     - |
| 11   | ret |     - |

## Лицензия
WTFPL 2.0
