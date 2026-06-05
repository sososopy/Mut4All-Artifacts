

enum class foo : unsigned char {
    FOO, BAR
};

foo v;

bool bar(foo old_v, foo new_v)
{
    return __atomic_compare_exchange_n(&v, &old_v, new_v, true,
                                       __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

