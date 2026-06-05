

#include <functional>

struct foo
{
        void f(int) const
        {
        }

        void g() const
        {
            auto mf = std::mem_fn(&foo::f);
            mf(this, 1);
        }
};
