
namespace std {
template<class> struct X {
  friend void f();
};
}

export module mod;
import "std.hpp";
export std::X<int> v;
