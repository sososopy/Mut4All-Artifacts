
#include <functional>

struct A  // line pointed-to by warning
{
        struct B
        {
                B& operator=(B&&) { return *this; }
        };

        B f;

        A() = default;
        A& operator=(A&& p) = default;  // where the method is declared
};

int main()
{
        A a;
        A b;

        b = std::move(a);
}
