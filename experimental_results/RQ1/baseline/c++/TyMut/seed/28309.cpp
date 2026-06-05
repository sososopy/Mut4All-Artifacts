
template<typename ... T>
struct DataStructure
{
};

template<size_t N>
struct StringLiteral 
{
    constexpr StringLiteral (const char (&str)[N]) 
    {
        std::copy_n(str, N, value); 
    }

    constexpr auto operator<=>(const StringLiteral&) const = default;
    constexpr bool operator==(const StringLiteral&) const  = default;
    static constexpr auto len = N; 
    char value[N];
};

