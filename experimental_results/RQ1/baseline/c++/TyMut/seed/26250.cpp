

struct A { void operator()(int); };

struct S
{
    template <typename T>
    auto foo(T t) -> decltype(a(t));

    A a;
};


int main()
{   
    S().foo(0);
}


