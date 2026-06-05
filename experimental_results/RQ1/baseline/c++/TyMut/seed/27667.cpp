template <typename... Args>
concept C = true;

struct S
{
    template <typename... Args>
    void f()
    requires C<Args...>;
};

template <typename... Args>
void S::f()
requires C<Args...>
{
}

