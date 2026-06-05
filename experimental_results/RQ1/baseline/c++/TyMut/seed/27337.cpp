
struct Foo {
    // void operator()(int) {}

    template<typename T>
    void operator()(T, T)
    {
        auto bar = [this](auto&& v){ operator()(v); };
    }
};

int main ()
{
    Foo{}(0,1);
    return 0;
}

