

extern void die(void) __attribute__((__noreturn__));

struct AutoDec{
    ~AutoDec() { }
};

bool testfn(void)
{
    AutoDec ad;
    return false;
    ((0) ? (void)0 : die());
}

