
#include <cstdlib>
#include <cstring>
#include <iostream>

struct Parent
{

    struct Child
    {
        int a;
        char b;
        char str[0]; ///< ASCIIZ
    } child;
};

//#define DONT_CRASH

int main(int argc, char** argv)
{
    char* buffer = new char[32768];

    Parent* parent = (Parent*) buffer;

    parent->child.a = 1;
    parent->child.b = 'a';

#ifdef DONT_CRASH
    Parent::Child& child = parent->child;
    char* childStr = child.str;
#else
    char* childStr = parent->child.str;
#endif

    std::cout << __USE_FORTIFY_LEVEL << std::endl;
    std::cout << __bos(childStr) << std::endl;

    size_t strLen = 4;
    std::strncpy(childStr, "test", strLen);
    if (childStr[strLen] not_eq '\0')
    {
        childStr[strLen] = '\0';
    }

    return 0;
}

