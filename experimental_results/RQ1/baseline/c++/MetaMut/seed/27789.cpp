
template <class T>
void foo()
{
    constexpr int x = 0;
    [&] {
        if constexpr (x) {}
    };
}
