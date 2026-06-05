
#include <type_traits>
#include <utility>

namespace xstd {

template<class CharT = char>
struct OStream
{
  OStream& operator<<(int);
  OStream& operator<<(short);
  OStream& operator<<(double);
  OStream& operator<<(const void*);
};

struct is_ostreamable_impl
{
  template<class CharT, class T, class =
    decltype(std::declval<OStream<CharT>&>() << std::declval<T>())>
  static std::true_type test(int);

  template<class, class>
  static std::false_type test(...);
};

template<class CharT, class T>
struct is_ostreamable
  : decltype(is_ostreamable_impl::test<CharT, T>(0))::type
{
};

template<class CharT, class T,
  typename std::enable_if<
    is_ostreamable<CharT, const T&>::value,
    bool
  >::type = false>
  OStream<CharT>& operator<<(OStream<CharT>&& os, const T& t)
{
  return os << t;
}

OStream<> out;

}

struct A {};

int main() {
  A a;
  xstd::out << 1;
  xstd::out << a;
}
