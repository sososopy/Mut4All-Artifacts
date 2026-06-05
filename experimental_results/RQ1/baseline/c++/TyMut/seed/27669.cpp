
struct A
{
    int _i;

    operator int&()
    {
        return _i;
    }
};

void f()    
{     
    A a;
    a++; // works
}

template <typename T> void f()
{
    A a;
    a++; // fails
}

