#ifndef CMD
#define CMD

#define TAKE_ARGS(stack)                                                        \
    double arg1 = 0, arg2 = 0;                                                  \
    Pop_(stack, &arg1);                                                         \
    Pop_(stack, &arg2);                                                         \


DEF_CMD(hlt, 0x0F, false,
        {
            return -1;
        }
    )

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
            double arg = get_arg(code_array,
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

DEF_CMD(pop,  0x08, false,
        {
            unsigned char reg_num = (unsigned char)code_array->code[++(*ip)];
            Pop_(&cpu->stack, cpu->regs + reg_num);
            return 0;
        })
#endif
