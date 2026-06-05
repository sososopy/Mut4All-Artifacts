

#include <cstddef>

class ThisCompilesOnGCC {
    static void impl(char const *);
public:
    template <typename T>
    [[nodiscard]] constexpr auto operator()(const T &t) const noexcept
    -> decltype(impl(t))
    {
        return impl(t);
    }
};
void thisCompilesOnGCC() {
    ThisCompilesOnGCC{}("42");
}

class Base01 {
protected:
    static void impl(char const *);
};
class ThisAlsoCompilesOnGCC : private Base01 {
    using Base01::impl;
public:
    template <typename T>
    [[nodiscard]] constexpr auto operator()(const T &t) const noexcept
    -> decltype(impl(t))
    {
        return impl(t);
    }
};
void thisAlsoCompilesOnGCC() {
    ThisAlsoCompilesOnGCC{}("42");
}

class Base02 {
protected:
    static void impl(char const *);
};
class ThisCompilesOnGCCToo : private Base02 {
    using Base02::impl;
    static void impl(int);
public:
    template <typename T>
    [[nodiscard]] constexpr auto operator()(const T &t) const noexcept
    -> decltype(impl(t))
    {
        return impl(t);
    }
};
void thisCompilesOnGCCToo() {
    ThisCompilesOnGCCToo{}("42");
}

#if defined (__clang__) || defined(SHOW_BUG)
// This compiles with clang - but not with gcc
class Base03 {
protected:
    static void impl(int);
};
class ThisDoesNotCompileOnGCC : private Base03 {
    using Base03::impl;
    static void impl(char const *);
public:
    template <typename T>
    [[nodiscard]] constexpr auto operator()(const T &t) const noexcept
    -> decltype(impl(t))
    {
        return impl(t);
    }
};
void thisDoesNotCompileOnGCC() {
    ThisDoesNotCompileOnGCC{}("42");
}
#endif
