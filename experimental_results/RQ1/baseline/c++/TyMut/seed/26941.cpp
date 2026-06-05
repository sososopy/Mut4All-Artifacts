
constexpr char c[] = "hello";
constexpr const char *p = c;

void f()
{
    static_assert(*(p-1) == 'h', "");
}
