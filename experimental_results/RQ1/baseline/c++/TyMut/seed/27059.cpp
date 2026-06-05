

#include <cstddef>
struct X_
        {
        operator std::nullptr_t() { return nullptr; }
        };
struct Y_
        {
        operator std::nullptr_t() { return nullptr; }
        };
int main()
{
    X_ x;
    Y_ y;
    bool z = x != y;

    return 0;
}

