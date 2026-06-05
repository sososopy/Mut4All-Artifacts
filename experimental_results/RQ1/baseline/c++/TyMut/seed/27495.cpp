
bool CPU_ProbePower9()
{
    // longjmp and clobber warnings. Volatile is required.
    volatile int result = true;

    volatile SigHandler oldHandler = signal(SIGILL, SigIllHandler);
    if (oldHandler == SIG_ERR)
        return false;

    volatile sigset_t oldMask;
    if (sigprocmask(0, NULLPTR, (sigset_t*)&oldMask))
    {
        signal(SIGILL, oldHandler);
        return false;
    }

    if (setjmp(s_jmpSIGILL))
        result = false;  ///// <= SIGILL here! /////
    else
    {
        // This is "darn r3, 0". We had to move away from the instrinsic 
        // because Clang and IBM XL C/C++ does not support the instrinsic.
#if __BIG_ENDIAN__
        __asm__ __volatile__ (".byte 0x7c, 0x60, 0x05, 0xe6  \n" : : : "r3");
#else
        __asm__ __volatile__ (".byte 0xe6, 0x05, 0x60, 0x7c  \n" : : : "r3");
#endif
        result = true;
    }

    sigprocmask(SIG_SETMASK, (sigset_t*)&oldMask, NULLPTR);
    signal(SIGILL, oldHandler);
    return result;
}

