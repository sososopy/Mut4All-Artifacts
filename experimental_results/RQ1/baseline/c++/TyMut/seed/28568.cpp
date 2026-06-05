
template<typename T>
struct Blerk
{
  template<typename Ptr>
    static constexpr auto
    type(Ptr p = Ptr(), int n = 0)
    -> decltype(noexcept(p[n]))
    { return false; }

  template<typename Ptr>
    static constexpr int
    type(...)
    { return 1; }

  decltype(type<T*>()) f() const;
};

struct S;
extern Blerk<S[]> p;
auto s = p.f();
struct S { };
