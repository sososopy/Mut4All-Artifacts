
template <typename T>
constexpr T PI = T(3.14);

template <typename T>
constexpr T TAU = 2 * PI<T>;



int main(int argc, char**)
{
    return TAU<double> * argc;
}

