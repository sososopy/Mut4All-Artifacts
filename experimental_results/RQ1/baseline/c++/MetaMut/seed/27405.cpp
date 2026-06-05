
struct non_exisiting {};

template <typename t>
auto hard_error(t in)
{
    static_assert(in, "Failure");
    return;
}

template <typename t>
    requires requires { hard_error(t{}); }
constexpr auto foo(t, non_exisiting);

template <typename t>
constexpr auto foo(t, bool) { return false; };

int main()
{
    foo(0, false);
};
