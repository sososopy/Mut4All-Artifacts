
class Base
{
public:
    virtual ~Base() {}
protected:
    void foo() const {}
};

template<typename T>
class CRTP : protected virtual T {};

template<typename T, typename U>
class X : protected virtual CRTP<U>
{
public:
    virtual void bar(const Base& inst) const
    {
        inst.foo();
    }
};

template<typename T>
class Y: protected virtual CRTP<T> {};

class Z : private Y<Base>, public X<Z, Base> {};

int main(int, char**)
{
    Z z;
    return 0;
}
