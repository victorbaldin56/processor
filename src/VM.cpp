#include <math.h>
#include <signal.h>
#include <stdlib.h>
#include "stack.h"
#include "colors.h"
#include "VM.h"
#include "codector.h"
#include "read_bin.h"
#include <assert.h>

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

static double *get_arg(const Code *codearr, size_t *ip, CPU *cpu);

static int cmp_double(const double a, const double b, const double eps);

/// @brief executes a command by code
static int cmd_exec(const Code *code, size_t *ip, CPU *cpu);

static int check_sign(const Code *code, size_t *ip);

static int vm_run(const Code *codearr);

ExecRes Process(char *filename) {
    assert(filename);

    Code codearr = {};

    if (!read_bin(filename, &codearr)) {
        perror("Processor");
        return FILE_NOT_FOUND;
    }

    int exec_code = vm_run(&codearr);
    CodeDtor(&codearr);

    if (exec_code != 0) {
        fprintf(stderr, "Incorrect file format\n");
        return NOT_VALID_FMT;
    }

    return EXEC_OK;
}

static int vm_run(const Code *codearr) {
    CODE_ASSERT(codearr);

    CPU cpu = {};
    CPU_Ctor(&cpu);

    size_t ip = 0;

    if (check_sign(codearr, &ip) != 0) return -1;

    for (; ip < codearr->size; ip++) {
        if (cmd_exec(codearr, &ip, &cpu) != 0) break;
    }

    CPU_Dtor(&cpu);
    return 0;
}

#define DEF_CMD(name, opcode, has_arg, ...)                         \
    case opcode:                                                    \
        {                                                           \
            ON_DEBUG(fprintf(stderr, "opcode = %02hhx, *ip = %zu\n",\
                                      cmd_code, *ip));              \
            __VA_ARGS__                                             \
        }

static int check_sign(const Code *codearr, size_t *ip) {
    CODE_ASSERT(codearr);
    assert(ip);

    if (*(int32_t *)codearr->code != SIGNATURE) return -1;
    (*ip) += sizeof(int32_t);

    if (codearr->code[*ip] != VERSION) return -1;
    (*ip)++;

    return 0;
}

static int cmd_exec(const Code *codearr, size_t *ip, CPU *cpu) {
    CPU_ASSERT(cpu);
    assert(ip);
    CODE_ASSERT(codearr);

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

static double *get_arg(const Code *codearr, size_t *ip, CPU *cpu) {
    CODE_ASSERT(codearr);
    assert(ip);
    CPU_ASSERT(cpu);

    double *res = NULL;
    size_t pos = *ip;

    switch (codearr->code[*ip] & (IMM | REG)) {
        case REG | IMM:
        {
            if (*ip + sizeof(double) + 1 >= codearr->size) raise(SIGSTOP);
            (*ip)++;
            cpu->regs[0] = 0;
            cpu->regs[0] += cpu->regs[codearr->code[*ip]];
            (*ip)++;
            cpu->regs[0] += *(double *)(codearr->code + *ip);
            (*ip) += (sizeof(double) - 1);
            res = cpu->regs;
            break;
        }

/*        case RAM | IMM:
        {
            if (*ip + 2 * sizeof(double) > codearr->size) raise(SIGSTOP);

            (*ip)++;



            break;
        } */

        case IMM:
        {
            if (*ip + sizeof(double) >= codearr->size) raise(SIGSTOP); // controls buffer overflow
            (*ip)++;
            res = (double *)(codearr->code + *ip);
            (*ip) += (sizeof(double) - 1);
            break;
        }

        case REG:
        {
            if (*ip + 1 >= codearr->size) raise(SIGSTOP);
            (*ip)++;
            ON_DEBUG(if (codearr->code[*ip] >= NUM_REGS) fprintf(stderr, "regidx = %hhu", codearr->code[*ip]));
            res = cpu->regs + codearr->code[*ip];
            ON_DEBUG(fprintf(stderr, "get_arg: reg = %lf\n", *res));
            break;
        }

        default:
        {
            raise(SIGSTOP);
        }
    }

    if (codearr->code[pos] & RAM) res = cpu->RAM + (size_t)*res;

    return res;
}

static int cmp_double(const double a, const double b, const double eps) {
    if (abs(b - a) < eps) {
        return 0;
    }

    return SIGN(b - a);
}
