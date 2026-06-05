
#include <string>

std::string GetHello()
{
    return std::string{"ello"};
}

int main()
{
    ("H" + GetHello());
}
