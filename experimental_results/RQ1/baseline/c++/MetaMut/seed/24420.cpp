
enum E {
    E_val  = 1,
};

inline constexpr E operator~(E e)
{
    return E(~static_cast<int>(e));
}

int main()
{
    int val = ~E_val;
    (void) val;
}
