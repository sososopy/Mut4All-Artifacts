
template <int N> static void k(decltype([]{ return 0; }()));
template <int N> static void k(decltype([]{ return 0; }()));
template <int N> static void k(int) {}

int main()
{
    k<0>(0);
}
