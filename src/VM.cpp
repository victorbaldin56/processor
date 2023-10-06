#include "stack.h"
#include "colors.h"
#include "../include/VM.h"
#include <assert.h>
#include <math.h>
#include <signal.h>
#include "../include/read_bin.h"
#include <stdlib.h>

#define SIGN(x) ((x > 0) - (x < 0))

#define POP(stk, val)                           \
    if (Pop(stk, val) == POP_EMPTY) {           \
        fprintf(stderr, "Pop: empty stack\n");  \
        abort();                                \
    }

#define PUSH(stk, val)                          \
    if (Push(stk, val) == REALLOC_FAILED) {     \
        fprintf(stderr, "Push: push failed\n"); \
        abort();                                \
    }

static const size_t MAXSTR = 1000; ///< max string lenght for byte-code
static const double EPS = 1e-7;    ///< floating point comparizon precision

static int cmp_double(const double a, const double b, const double eps);

// executes a command by code
static int cmd_exec(const unsigned char *opcode, Stack *stk, double reg[]);

static void vm_run(Code *code_array);

void VM_Proc(FILE *input_file) {
    assert(input_file);

    Code code_array = {};
    read_bin(input_file, &code_array);
    vm_run(&code_array);

    return;
}

static void vm_run(Code *code_array) {
    assert(code_array);
    assert(code_array->code);

    Stack stk = {};
    StackCtor(&stk);

    double reg[NUM_REGS] = {};

    for (size_t ip = 0; ip < code_array->size; ip++) {
        int nargs = cmd_exec(code_array->code + ip, &stk, reg);

        if (nargs == -1) {
            StackDtor(&stk);
            return;
        }

        ip += nargs;
    }

    StackDtor(&stk);
    free(code_array->code);
    return;
}

static int cmd_exec(const unsigned char *opcode, Stack *stk, double reg[]) {
    assert(stk);
    assert(reg);
    assert(opcode);

    double arg1 = 0, arg2 = 0;

    ON_DEBUG(printf("opcode = %hhx\n", *opcode & 15));
    switch (*opcode & 15) {
        // hlt
        case -1:
            return -1;
        // in
        case 0:
            scanf("%lf", &arg1);
            PUSH(stk, arg1);
            break;
        // out
        case 1:
            POP(stk, &arg1);
            printf("%lf\n", arg1);
            break;
        // push
        case 2:
            // push constant
            if (*opcode & 0x10) {
                PUSH(stk, *((double *)(opcode + 1)));
            }

            // push from register
            if (*opcode & 0x20) {
                PUSH(stk, reg[*(opcode + 1)]);
            }

            return 1;

            break;
        // add
        case 3:
            POP(stk, &arg1);
            POP(stk, &arg2);
            PUSH(stk, arg1 + arg2);
            break;
        // sub
        case 4:
            POP(stk, &arg1);
            POP(stk, &arg2);
            PUSH(stk, arg2 - arg1);
            break;
        // mult
        case 5:
            POP(stk, &arg1);
            POP(stk, &arg2);
            PUSH(stk, arg1 * arg2);
            break;
        // div
        case 6:
            POP(stk, &arg1);
            POP(stk, &arg2);

            if (!cmp_double(arg1, 0, EPS)) {
                raise(SIGFPE);
            }

            PUSH(stk, arg2 / arg1);
            break;
        // sqrt
        case 7:
            POP(stk, &arg1);

            if (arg1 < 0) {
                raise(SIGFPE);
            }

            PUSH(stk, sqrt(arg1));
            break;
        // pop
        case 8:
            if (!(*opcode & 0x8)) {
                fprintf(stderr, "No register for pop\n");
                abort();
            }

            POP(stk, reg + *(opcode + 1));
            return 1;

        // default
        default:
            fprintf(stderr, "Unknown command\n");
            abort();
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
