
#include <stdio.h>

template<class T0, class T1>
struct TCC {
        static const T0 tvalue = 100;
};

struct CC {
        static const int value = 200;
};

int main()
{
    CC cc;
    TCC<int, int> tcc;

        printf("tcc.value1 = %u \n", tcc.tvalue);
        printf("cc.value1 = %u \n", cc.value);
}
