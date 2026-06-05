
template <class T, class U> 
concept bool Same() 
{
    return true;
}

template <class T> struct WrapT {T t;};

template <class T>
concept bool Destructible()
{
    return requires(T t, const T ct, WrapT<T>& wt)
    {
        {wt.~WrapT()} noexcept;
        {&t} -> Same<T*>; // #1
        //{&t} -> T*; // #2
    };
}

template <Destructible T>
void f() {}

struct Y {private: ~Y();};

int main()
{
    f<Y>();
}

