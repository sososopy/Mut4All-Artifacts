
template <typename T, typename U>
struct base {
  
  template <typename V>
  struct derived;
 
};
  
template <typename T, typename U>
template <typename V>
struct base<T, U>::derived : public base<T, V> {
};

// This (wrong?) alias declaration provoques the crash.
template <typename T, typename U, typename V>
using derived = base<T, U>::derived<V>;

// This one works:
// template <typename T, typename U, typename V>
// using derived = typename base<T, U>::template derived<V>;

template <typename T>
void f() {
  derived<T, bool, char> m{};
  (void) m;
}

int main() {
  f<int>();
}

