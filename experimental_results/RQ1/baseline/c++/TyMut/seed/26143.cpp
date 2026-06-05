
#ifdef IS_INCLUDE_FILE
#pragma clang system_header

#define M(a) \
    __extension__({ int v = a; v; })

#else
#define IS_INCLUDE_FILE
#include "clang.cpp"

int main()
{
    int i = M(M(1));
}
#endif


