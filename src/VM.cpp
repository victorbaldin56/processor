#include <assert.h>
#include <math.h>
#include <signal.h>
#include <stdlib.h>
#include "stack.h"
#include "colors.h"
#include "VM.h"
#include "codector.h"
#include "read_bin.h"

#define SIGN(x) ((x > 0) - (x < 0))

#define Pop_(stk, val)                          \
    if (Pop(stk, val) == POP_EMPTY) {           \
        fprintf(stderr, "Pop: empty stack\n");  \
        abort();                                \
    }

#define Push_(stk, val)                         \
    if (Push(stk, val) == REALLOC_FAILED) {     \
        fprintf(stderr, "Push: push failed\n"); \
        abort();                                \
    }

static const size_t MAXSTR = 1000; ///< max string lenght for byte-code
static const double EPS = 1e-7;    ///< floating point comparizon precision

static double get_arg(const Code *codearr, size_t *ip, CPU *cpu);

static int cmp_double(const double a, const double b, const double eps);

/// @brief executes a command by code
static int cmd_exec(const Code *code, size_t *ip, CPU *cpu);

static void vm_run(const Code *codearr);

int Process(char *filename) {
    assert(filename);

    Code codearr = {};

    if (!read_bin(filename, &codearr)) return -1;

    vm_run(&codearr);
    CodeDtor(&codearr);

    return 0;
}

static void vm_run(const Code *codearr) {
    assert(codearr);
    assert(codearr->code);

    CPU cpu = {};
    CPU_Ctor(&cpu);

    for (size_t ip = 0; ip < codearr->size; ip++) {
        if (cmd_exec(codearr, &ip, &cpu)) break;
    }

    CPU_Dtor(&cpu);
}

#define DEF_CMD(name, opcode, has_arg, ...)     \
    case opcode:                                \
        __VA_ARGS__                             \

static int cmd_exec(const Code *codearr, size_t *ip, CPU *cpu) {
    assert(cpu);
    assert(ip);
    CODE_ASSERT(codearr);
    assert(cpu->regs);
    STACK_ASS(&cpu->stack);

    unsigned char cmd_code = codearr->code[*ip];

    switch (cmd_code & CMD) {
        #include "commands.h"

        default:
        {
            raise(SIGSTOP);
        }
    }

    return -1;
}

#undef DEF_CMD

static double get_arg(const Code *codearr, size_t *ip, CPU *cpu) {
    assert(codearr);
    assert(ip);
    assert(cpu);

    double res = 0;

    if (codearr->code[*ip] & IMM) {
        (*ip)++;
        res = *(double *)(codearr->code + *ip);
        (*ip) += sizeof(double);
    }

    if (codearr->code[*ip] & REG) {
        res = cpu->regs[codearr->code[++(*ip)]];
    }

    return res;
}

static int cmp_double(const double a, const double b, const double eps) {
    if (abs(b - a) < eps) {
        return 0;
    }

    return SIGN(b - a);
}
