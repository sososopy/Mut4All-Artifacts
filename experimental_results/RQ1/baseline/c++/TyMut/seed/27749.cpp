
#include <string>
struct base
{
    std::string bla;
};

struct foo : base
{
    int a;
};

base makeBase();
int getInt();

foo create()
{
    return foo{ makeBase(), getInt() };
}
