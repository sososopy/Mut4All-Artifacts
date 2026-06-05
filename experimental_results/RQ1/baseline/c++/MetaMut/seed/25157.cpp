


template <typename T>
struct StrictType
{
  template <typename U>
  StrictType(U) = delete;

  constexpr StrictType(T t) : m_value(t) {}

  T m_value {};
};

struct D2;

struct D1 : StrictType<int>
{
    using StrictType<int>::StrictType;
    D1(D2);
};

struct D2 : D1
{
    using D1::D1;
};

D1::D1(D2 v) : D1(v.m_value) {}
