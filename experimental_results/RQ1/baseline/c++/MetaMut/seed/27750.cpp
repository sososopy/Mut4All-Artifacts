
using size_t = decltype(sizeof(0));

struct Str {
    template <size_t N>
    explicit(N > 7) Str(char const (&str)[N]);
};

#ifdef OUT_OF_LINE
template <size_t N>
Str::Str(char const(&str)[N]) { }
#endif

Str a = "short";
Str b = "not so short";

