
template<typename T>
constexpr void example()
{
    constexpr auto result = requires { typename T::value_type; };
    static_assert(not result);
}

int main()
{
   struct s{};
   example<s>();
}
