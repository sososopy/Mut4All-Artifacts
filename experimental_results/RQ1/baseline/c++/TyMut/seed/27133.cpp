
void test()
{
    []()
#if 1
    __attribute__((no_sanitize_undefined))
#endif
    ->int {
        return 0;
    };
}
