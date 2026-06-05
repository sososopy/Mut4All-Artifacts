
#include <typeinfo>
class Foobar {};
extern "C" { const char *makename(); }
const char *makename() { Foobar f; return typeid(f).name(); }
» cat m.cpp
#include <cstdio>
#include <dlfcn.h>
int main()
{
        auto hnd = dlopen("./libx.so", RTLD_NOW);
        auto f = reinterpret_cast<const char *(*)()>(dlsym(hnd, "makename"));
        auto name = f();
        printf("%s\n", name);
        dlclose(hnd);
        printf("%s\n", name);
}
