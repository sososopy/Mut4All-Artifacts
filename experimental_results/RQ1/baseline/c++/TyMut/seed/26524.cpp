
void f(const char * const str) __attribute__((access(read_only, 1)));
void f(const char * const str)
{
    __builtin_puts(str);
}

int main()
{
    const char * a = nullptr;
    f(a);
}

