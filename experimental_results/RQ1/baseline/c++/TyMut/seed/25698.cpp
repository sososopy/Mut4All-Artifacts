
struct foo {
    template<typename T>
    void bar(T) &;

    template<typename T>
    void bar(T) &&;
};

template<typename T>
void foo::bar(T) & {}

template<typename T>
void foo::bar(T) && {}

int main()
{
    foo f;
    // error: call of overloaded 'bar(int)' is ambiguous
    f.bar(0);
}
//---

