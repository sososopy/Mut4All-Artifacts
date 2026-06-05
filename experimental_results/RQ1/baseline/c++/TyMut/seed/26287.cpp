
#include <type_traits>

template<bool b>
struct F {
  F() noexcept(b) {}
  F& operator=(F&&) noexcept(b) {return *this;}
};

struct Simple {
  Simple() noexcept {}
  Simple& operator=(Simple&&) noexcept {return *this;}
};

struct X {
  X() noexcept = default;
  X& operator=(X&&) noexcept = default;
  F<true> f;
};
struct X2 {
  X2() /* noexcept */ = default;
  X2& operator=(X2&&) /* noexcept */ = default;
  F<true> f;
};
struct X3 {
  X3() noexcept = default;
  X3& operator=(X3&&) noexcept = default;
  Simple f;
};

static_assert(std::is_nothrow_constructible<X>::value, "");
static_assert(std::is_nothrow_move_assignable<X>::value, "");
static_assert(std::is_nothrow_constructible<X2>::value, "");
static_assert(std::is_nothrow_move_assignable<X2>::value, "");
static_assert(std::is_nothrow_constructible<X3>::value, "");
static_assert(std::is_nothrow_move_assignable<X3>::value, "");

