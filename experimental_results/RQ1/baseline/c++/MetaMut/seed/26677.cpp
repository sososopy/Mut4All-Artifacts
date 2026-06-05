
int foo(int x)
{
    return x;
}
 
template <typename T>
concept bool C1 = requires (T x) {
    {foo(x)} -> int&;
};

template <typename T>
concept bool C2 = requires (T x) {
    {foo(x)} -> void;
};
 
static_assert( C1<int> );
static_assert( C2<int> );

