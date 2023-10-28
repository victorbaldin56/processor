#ifndef CMD
#define CMD

#define TAKE_ARGS(stack)                                    \
    double arg1 = 0, arg2 = 0;                              \
    Pop_(stack, &arg1);                                     \
    Pop_(stack, &arg2);                                     \

#define COND_JMP(expr) jump(codearr, ip, cpu, expr)

DEF_CMD(hlt, 0x10, false,
        {
            return -1;
        })

DEF_CMD(in,  0x00, false,
        {
            double arg = 0;
            scanf("%lf", &arg);
            Push_(&cpu->stack, arg);
            return 0;
        })


DEF_CMD(out, 0x01, false,
        {
            double arg = 0;
            Pop_(&cpu->stack, &arg);
            printf("%lf\n", arg);
            return 0;
        })

DEF_CMD(push, 0x02, true,
        {
            double arg = *get_arg(codearr,
                                  ip, cpu);
            Push_(&cpu->stack, arg);
            return 0;
        })

DEF_CMD(add,  0x03, false,
        {
            TAKE_ARGS(&cpu->stack);
            Push_(&cpu->stack, arg1 + arg2);
            return 0;
        })

DEF_CMD(sub, 0x04, false,
        {
            TAKE_ARGS(&cpu->stack);
            Push_(&cpu->stack, arg2 - arg1);
            return 0;
        })

DEF_CMD(mul, 0x05, false,
        {
            TAKE_ARGS(&cpu->stack);
            Push_(&cpu->stack, arg1 * arg2);
            return 0;
        })

DEF_CMD(div, 0x06, false,
        {
            TAKE_ARGS(&cpu->stack);

            if (!cmp_double(arg2, 0, EPS)) raise(SIGFPE);

            Push_(&cpu->stack, arg2 / arg1);
            return 0;
        })

DEF_CMD(sqrt, 0x07, false,
        {
            double arg = 0;
            Pop_ (&cpu->stack, &arg);

            if (arg < 0) raise(SIGFPE);

            Push_(&cpu->stack, sqrt(arg));
            return 0;
        })

DEF_CMD(pop,  0x08, true,
        {
            double *popaddr = get_arg(codearr, ip, cpu);

            Pop_(&cpu->stack, popaddr);
            return 0;
        })

DEF_CMD(jmp,  0x09, true,
        {
            COND_JMP(true);

            return 0;
        })

DEF_CMD(jae,  0x0A, true,
        {
            TAKE_ARGS(&cpu->stack);

            COND_JMP(cmp_double(arg1, arg2, EPS) <= 0);

            return 0;
        })

DEF_CMD(jbe,  0x0B, true,
        {
            TAKE_ARGS(&cpu->stack);

            COND_JMP(cmp_double(arg1, arg2, EPS) >= 0);

            return 0;
        })

DEF_CMD(ja,   0x0C, true,
        {
            TAKE_ARGS(&cpu->stack);

            COND_JMP(cmp_double(arg1, arg2, EPS) < 0);

            return 0;
        })

DEF_CMD(jb,   0x0D, true,
        {
            TAKE_ARGS(&cpu->stack);

            COND_JMP(cmp_double(arg1, arg2, EPS) > 0);

            return 0;
        })

DEF_CMD(je,   0x0E, true,
        {
            TAKE_ARGS(&cpu->stack);

            COND_JMP(cmp_double(arg1, arg2, EPS) == 0);

            return 0;
        })

DEF_CMD(call, 0x0F, true,
        {
            ON_DEBUG(fprintf(stderr, "there is a call, current ip = %zu\n", *ip));
            Push_(&cpu->callstack, (double)(*ip));

            COND_JMP(true);

            return 0;
        })

DEF_CMD(ret,  0x11, false,
        {
            double addr = 0;

            Pop_(&cpu->callstack, &addr);

            *ip = (size_t)(addr + sizeof(double));
            ON_DEBUG(fprintf(stderr, "return back, *ip = %zu\n", *ip));

            return 0;
        })


#endif
