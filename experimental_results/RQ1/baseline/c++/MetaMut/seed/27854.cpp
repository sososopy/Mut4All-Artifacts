
#include <memory>
#include <cstdio>
struct D {
        D() { printf("*D\n"); }
        ~D() { printf("~D\n"); }
        auto operator<=>(const D &) = default;
};
int main()
{
        D d;
        d <=> d;
}

