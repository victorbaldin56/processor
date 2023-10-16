#include "stack.h"
#include "colors.h"
#include "../include/VM.h"
#include <assert.h>
#include <math.h>
#include <signal.h>
#include "../include/read_bin.h"
#include <stdlib.h>

#define SIGN(x) ((x > 0) - (x < 0))

#define Pop_(stk, val)                           \
    if (Pop(stk, val) == POP_EMPTY) {           \
        fprintf(stderr, "Pop: empty stack\n");  \
        abort();                                \
    }

#define Push_(stk, val)                          \
    if (Push(stk, val) == REALLOC_FAILED) {     \
        fprintf(stderr, "Push: push failed\n"); \
        abort();                                \
    }

static const size_t MAXSTR = 1000; ///< max string lenght for byte-code
static const double EPS = 1e-7;    ///< floating point comparizon precision

static int cmp_double(const double a, const double b, const double eps);

/// @brief executes a command by code
static int cmd_exec(const double *code, ssize_t ip, CPU *cpu);

static void vm_run(Code *code_array);

void VM_Proc(char *filename) {
    assert(filename);

    Code code_array = {};
    read_bin(filename, &code_array);
    vm_run(&code_array);

    return;
}

static void vm_run(Code *code_array) {
    assert(code_array);
    assert(code_array->code);

    CPU cpu = {};
    CPU_Ctor(&cpu);

    for (ssize_t ip = 0; ip < code_array->size; ip++) {
        int nargs = cmd_exec(code_array->code, ip, &cpu);

        if (nargs == -1) {
            break;
        }

        ip += nargs;
    }

    CPU_Dtor(&cpu);
    free(code_array->code);
    return;
}

static int cmd_exec(const double *code, ssize_t ip, CPU *cpu) {
    assert(cpu);
    assert(cpu->regs);
    STACK_ASS(&cpu->stack);
    assert(ip >= 0);

    double arg1 = 0, arg2 = 0;
    char opcode = *(char *)(code + ip);

    ON_DEBUG(printf("*opcode = %hhx\n", opcode & CMD));

    switch (opcode & CMD) {
        case HALT:
            return -1;

        case IN:
            scanf("%lf", &arg1);
            Push_(&cpu->stack, arg1);
            break;

        case OUT:
            Pop_(&cpu->stack, &arg1);
            printf("%lf\n", arg1);
            break;

        case PUSH:
            // push constant
            if (opcode & IMM) {
                Push_(&cpu->stack, code[ip + 1]);
            }

            // push from register
            if (opcode & REG) {
                Push_(&cpu->stack, cpu->regs[*(char *)(code + ip + 1)]);
            }

            return 1;

        case ADD:
            Pop_(&cpu->stack, &arg1);
            Pop_(&cpu->stack, &arg2);
            Push_(&cpu->stack, arg1 + arg2);
            break;

        case SUB:
            Pop_(&cpu->stack, &arg1);
            Pop_(&cpu->stack, &arg2);
            Push_(&cpu->stack, arg2 - arg1);
            break;

        case MULT:
            Pop_(&cpu->stack, &arg1);
            Pop_(&cpu->stack, &arg2);
            Push_(&cpu->stack, arg1 * arg2);
            break;

        case DIV:
            Pop_(&cpu->stack, &arg1);
            Pop_(&cpu->stack, &arg2);

            if (!cmp_double(arg1, 0, EPS)) {
                raise(SIGFPE);
            }

            Push_(&cpu->stack, arg2 / arg1);
            break;

        case SQRT:
            Pop_(&cpu->stack, &arg1);

            if (arg1 < 0) {
                raise(SIGFPE);
            }

            Push_(&cpu->stack, sqrt(arg1));
            break;

        case POP:
            // ON_DEBUG(StackDump(stk, STACK_OK, __FILE__, __LINE__));

            if (!(opcode & REG)) {
                fprintf(stderr, "No register for pop\n");
                abort();
            }

            char reg_num = *(char *)(code + ip + 1);

            ON_DEBUG(printf("reg_num = %zu\n", reg_num));

            Pop_(&cpu->stack, cpu->regs + reg_num);
            return 1;
            break;

        // default:
        //     fprintf(stderr, "Unknown command\n");
        //     abort();
    }

    return 0;
}

static int cmp_double(const double a, const double b, const double eps) {
    assert(isfinite(a));
    assert(isfinite(b));
    assert(isfinite(eps));

    if (abs(b - a) < eps) {
        return 0;
    }

    return SIGN(b - a);
}
