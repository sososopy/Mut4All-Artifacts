
constexpr void f()
{
    delete[] new int;
    delete new int[1];
}
constexpr int x{([]() consteval { f(); }(), 0)};
