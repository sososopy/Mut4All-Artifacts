
#include <compare>

class MyClass
{
    int mValue;

 public:
    MyClass(int value): mValue(value) {}

    bool operator<=>(const MyClass&) const = default;
};

int main()
{
    MyClass a = 10, b = 15;
    return (a < b);
}
