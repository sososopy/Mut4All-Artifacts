struct A {
    A();
};

template <class T>
struct opt
{
    union Data {
        
        constexpr Data() : x{} {}
        constexpr ~Data() {}

        char x;
        T data;
    };

    constexpr opt() : data{} {}
    constexpr ~opt() { if (engaged) data.data.~T(); }

    Data data;
    bool engaged = false;
};

consteval void foo()
{
    opt<A> a;
}

int main()
{
    foo();
}