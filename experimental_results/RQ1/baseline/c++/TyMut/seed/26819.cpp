
#include <cassert>

template <class T, class U>
void
test(const U& uo)
{
    T t{uo};
    U u{t};
    assert(u == uo);
    t = T{uo};
    u = U{t};
    assert(u == uo);
}

class X
{
    unsigned char data_;
public:
    explicit X(unsigned data)
        : data_(data)
        {}

    explicit operator unsigned() const {return data_;}

    friend bool operator==(const X& x, const X& y)
    {
        return x.data_ == y.data_;
    }
};

int
main()
{
    test<unsigned>(X{3});
}

