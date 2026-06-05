
#include <cstring>
void foo()
{
    const char* a = "Hello Whirled!";
    auto b = strchr(a, 'W');
    b = a;
}
