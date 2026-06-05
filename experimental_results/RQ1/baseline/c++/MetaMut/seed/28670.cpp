

#include <algorithm> // copy_n and size_t
static constexpr unsigned hash(const char* s, std::size_t length)
{
    s=s;
    return length;
}
template<std::size_t N>
struct fixed_string
{
    constexpr fixed_string(const char (&s)[N]) { std::copy_n(s, N, str); }
    consteval const char* data() const { return str; }
    consteval std::size_t size() const { return N-1; }
    char str[N];
};
template<unsigned expected_hash, fixed_string... s>
static consteval void VerifyHash()
{
    (
      [](){static_assert(hash(s.data(), s.size()) == expected_hash);}()
    ,...);
    // ^ Falsely reports that there are no unexpanded parameter packs,
    //   while there definitely is ("s" is used).
}
void foo()
{
    VerifyHash<5, "khaki", "plums">();
}

