#include "stack.h"
#include "colors.h"
#include "../include/VM.h"
#include <assert.h>
#include <math.h>
#include <signal.h>

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
static int cmd_exec(const char *cmd, Stack *stk);

void VM_Proc(FILE *input_file) {
    assert(input_file);

    Stack stk = {};
    StackCtor(&stk);

    char cmd[MAXLEN] = {};

    while (fgets(cmd, MAXLEN, input_file)) {
        if (cmd_exec(cmd, &stk)) {
            StackDtor(&stk);
            return;
        }
    }

    StackDtor(&stk);
    return;
}

static int cmd_exec(const char *cmd, Stack *stk) {
    assert(cmd);
    assert(stk);

    int cmd_code = 0;

    if (sscanf(cmd, "%d", &cmd_code) <= 0) {
        fprintf(stderr, "Unknown command\n");
        abort();
    }

    double a = 0, b = 0;

    switch (cmd_code) {
        // hlt
        case -1:
            return -1;
        // in
        case 0:
            scanf("%lf", &a);
            PUSH(stk, a);
            break;
        // out
        case 1:
            POP(stk, &a);
            printf("%lf\n", a);
            break;
        // push
        case 2:
            sscanf(cmd, "%*s %lf", &a);
            PUSH(stk, a);
            break;
        // add
        case 3:
            POP(stk, &a);
            POP(stk, &b);
            PUSH(stk, a + b);
            break;
        // sub
        case 4:
            POP(stk, &a);
            POP(stk, &b);
            PUSH(stk, b - a);
            break;
        // mult
        case 5:
            POP(stk, &a);
            POP(stk, &b);
            PUSH(stk, a * b);
            break;
        // div
        case 6:
            POP(stk, &a);
            POP(stk, &b);

            if (!cmp_double(a, 0, EPS)) {
                raise(SIGFPE);
            }

            PUSH(stk, b / a);
            break;
        // sqrt
        case 7:
            POP(stk, &a);

            if (a < 0) {
                raise(SIGFPE);
            }

            PUSH(stk, sqrt(a));
            break;
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
