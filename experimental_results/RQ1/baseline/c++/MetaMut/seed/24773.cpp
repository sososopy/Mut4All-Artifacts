
#include <new>
#include <cstdio>

int main()
{
    int sz = -1;
    char buf[9];
    char* res = new (buf) char[sz];
    std::puts(res == (char*) nullptr? "correct" : "wrong");
}

