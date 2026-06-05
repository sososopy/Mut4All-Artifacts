
#include <cstddef>
#include <iostream>
#include <string_view>
#include <utility>

template <std::size_t N>
struct json_name {
  static_assert(N > 0);
  char const m_data[N]{};

 private:
  template <std::size_t... Is>
  constexpr json_name(char const (&ptr)[N], std::index_sequence<Is...>)
      : m_data{ptr[Is]...} {}

 public:
  constexpr json_name(char const (&ptr)[N])
      : json_name(ptr, std::make_index_sequence<N>{}) {}

  // Needed for copy_to_iterator
  [[nodiscard]] constexpr char const *begin() const { return m_data; }

  // Needed for copy_to_iterator
  [[nodiscard]] constexpr char const *end() const {
    return m_data + static_cast<ptrdiff_t>(size());
  }

  [[nodiscard]] static constexpr std::size_t size() noexcept { return N - 1; }

  template <std::size_t M>
  constexpr bool operator==(json_name<M> const &rhs) const {
    if (N != M) {
      return false;
    }
    for (std::size_t n = 0; n < N; ++n) {
      if (m_data[n] != rhs.m_data[n]) {
        return false;
      }
    }
    return true;
  }

  constexpr operator std::string_view() const {
    return std::string_view(m_data, size());
  }
};
template <typename... Chars>
json_name(Chars...) -> json_name<sizeof...(Chars)>;

template <std::size_t N>
json_name(char const (&)[N]) -> json_name<N>;

#ifdef USE_CNTTP
#define JSONNAMETYPE json_name
#else
#define JSONNAMETYPE char const *
#endif

template <JSONNAMETYPE Name, typename U>
struct X : std::false_type {
  static constexpr std::string_view name = Name;
};

template <JSONNAMETYPE Name>
struct X<Name, bool> : std::true_type {
  static constexpr std::string_view name = Name;
};

int main() {
  static constexpr char const A[] = "A";  
  static constexpr char const B[] = "B";  
  auto a = X<A, int>::name;
  auto b = X<B, bool>::name;
  std::cout << a << b << '\n';
}
