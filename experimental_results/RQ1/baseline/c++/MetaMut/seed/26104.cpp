
#include <cstdio>

void f()
{
        static const size_t buf_size = 30;
        char fmt_buf[buf_size];
        auto my_fmt_f = [&fmt_buf](int a, int b) -> char * {
                snprintf(fmt_buf, buf_size, "AA %d BB %d CC\n", a, b);
                return fmt_buf;
        };
        puts(my_fmt_f(10, 20));
        puts(my_fmt_f(30, 2));
}

int main()
{
        f();
        return 0;
}
