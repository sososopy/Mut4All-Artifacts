

template <class T, class U>
constexpr bool Same = __is_same_as(T, U);

template <class T>
concept bool C = requires(T& t) { t.f(); };

template <class T>
concept bool D = C<T> && requires(T& t) { requires Same<T, decltype(t.f())>; };
// NB: D subsumes C

struct X {};
struct Y { int f(); };
struct Z { Z f(); };

// Y satisfies C but not D
static_assert(C<Y>);
static_assert(!D<Y>);

// Z satisfies both C and D
static_assert(C<Z>);
static_assert(D<Z>);

template <class T>
struct foo {
  void g(T); // #1

  int g(T)   // #2
    requires C<T>;

  T g(T)     // #3
    requires D<T>;
};

static_assert(Same<decltype(foo<X>{}.g(X{})), void>); // calls #1
static_assert(Same<decltype(foo<Y>{}.g(Y{})), int>);  // calls #2
static_assert(Same<decltype(foo<Z>{}.g(Z{})), Z>);    // error: ambiguous

