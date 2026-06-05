

#include <functional>

typedef std::function<auto (int, int) -> int> fn_type;

auto f1 = [](int,int){return 0;};

struct B
{
    typedef struct
        {
            fn_type _f = f1;
    } A;
    B(A a = A()){}
};
