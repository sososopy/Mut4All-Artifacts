

template <typename T>
struct type_identity {
  using type = T;
};

template <typename T>
using type_identity_t = typename type_identity<T>::type;

constexpr int check() {
  return {};
}

template <typename... Args>
class basic_format_string {
 public:
  template <typename S>
  consteval basic_format_string(const S& s) {
    if (!check()) {
    }
  }
};

template <typename... Args>
using format_string = basic_format_string<char, type_identity_t<Args>...>;

template <typename... Args>
void test(format_string<Args...> format_str, const Args&... args) {}

enum { Blue };

void g() {
  test("", Blue);
}


