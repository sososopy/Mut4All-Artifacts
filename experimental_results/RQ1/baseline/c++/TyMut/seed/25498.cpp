
template<class C, void fn(C)>
struct type
{
    virtual void f(int arg)
    {
        fn(arg);
    }
};

type<int, nullptr> g;
