
#include <stdio.h>

class A
{
public:
    A() {
        printf("A()\n");
    }
    virtual void b() const = 0;
};

int main()
{
    const A& a{};
    a.b();
    return 0;
}

