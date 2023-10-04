# VM_CPU
Virtual machine for byte-code programs execution written in C.

## Usage

### Building
This program uses a library from my previous
project [Stack](https://github.com/victorbaldin56/Stack).
So follow this steps to build from source code:
1. Clone Stack repository: ```git clone https://github.com/victorbaldin56/Stack.git```
2. Build Stack library: ```make -f Makelib```
3. Clone the projetc's repo: ```git clone https://github.com/victorbaldin56/VM_CPU.git```
4. In Makefile, change the ```INCLUDE_DIR```, ```LIB``` and ```L_DIR``` variables
to your own path to the cloned Stack repo.
5. Run ```make``` to build! The output will be placed
to ```./build``` directory by default.

### Supported commands
This virtual CPU currently
supports the entire list of the following:
| Command code | Command name | What command does |
| :----------: | :----------: | :---------------: |
| -1           | hlt          | Halt; stop the program |
| 0            | in           | Take a value from standart input and push it to stack |
| 1            | out          | Pop a last value from stack and print it |
| 2            | push         | Push an argument following to the command code to stack |
| 3            | add          | Pop 2 last values from stack and push the sum to stack |
| 4            | sub          | Substract 2 last numbers from stack |
| 5            | mult         | Multiply |
| 6            | div          | Divide; raise signal if denominator is zero |
| 7            | sqrt         | Take a square root |
