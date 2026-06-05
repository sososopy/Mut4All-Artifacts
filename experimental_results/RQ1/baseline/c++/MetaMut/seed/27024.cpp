
#include <array>

template <typename T, std::size_t N>
class FixedVector : protected std::array<T, N>
{
public:
  typedef typename std::array<T, N> base;
  typedef uint8_t internal_size_type;

  constexpr FixedVector()
  : base()
  , size_(0u)
  {}

private:
  internal_size_type size_;
};

template <typename T>
class ptr
{
public:
  typedef T element_type;
  constexpr ptr(T* p = nullptr) noexcept : px_(p) {}

private:
  T* px_;
};

class ForwardDeclaredClass;

void func()
{
  FixedVector<ptr<ForwardDeclaredClass>, 1> var;
}

