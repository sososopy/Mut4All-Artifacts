
template<typename T> constexpr bool f(T a)
{
    return a == 3;
}
extern template bool f<int>(int);
bool g(int x) { return f(x); }
template bool f<int>(int);
int main() { return g(4); }

