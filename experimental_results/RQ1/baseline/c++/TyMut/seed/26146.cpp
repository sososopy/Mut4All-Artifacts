
template <typename T>
constexpr T PI = T(3);

template <typename T>
static T foo()
{
    return PI<T>;
}

int main()
{
    foo<float>();
}

