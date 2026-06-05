

struct function {
  template<typename F>
    function(F);
};

template <typename T1, typename T2>
auto sum(T1 x, T2 y)
{
    return x + y;
}

template <typename T1, typename T2, typename... Targ>
auto sum(T1 x, T2 y, Targ... args)
{
    return sum(x + y, args...);
}

int main()
{
    //(void)sum<int, int, int>;
    function f = sum<int, int, int>;
}
