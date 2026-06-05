
#include <cstdio>

struct Foo
{
        Foo() {
                printf("Foo()\n");
        }
        Foo(const Foo& x) = delete;
        Foo(Foo&& x) = delete;
        Foo& operator=(const Foo&) = delete;
        Foo& operator=(Foo&&) = delete;
        ~Foo() {
                printf("~Foo()\n");
        }
};

int main() {
        Foo     base_foo;
        const auto& my_foo = (true ? base_foo : throw "");
        printf("Selected a foo\n");
        return 0;
}
