template<char c>
constexpr bool f(char d) {
    return 'a' <= c && c <= 'z' ? (d | 0x20) == c :
        'A' <= c && c <= 'Z' ? (d & ~0x20) == c :
        d == c;
}
static_assert(f<'p'>('P'));

