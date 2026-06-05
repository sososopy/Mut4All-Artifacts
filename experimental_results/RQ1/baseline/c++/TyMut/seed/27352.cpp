
#include <iostream>
#include <string>

// FixedString from https://www.reddit.com/r/cpp/comments/bhxx49/c20_string_literals_as_nontype_template/
template<unsigned N>
struct FixedString {
  char buf[N + 1]{};
  constexpr FixedString(char const* s) {
    for (unsigned i = 0; i != N; ++i) buf[i] = s[i];
  }
  constexpr operator char const*() const { return buf; }
};
template<unsigned N> FixedString(char const (&)[N]) -> FixedString<N - 1>;

// own code

// Static url part
template <FixedString Path>
struct Slug {
  static constexpr char const* path = Path;
};

// Dynamic url part
template <typename Value>
struct Capture {
  using value_t = Value;
};

// Route
template <typename... Args>
struct Route;

template<typename ValueT, typename... Rest>
struct Route<Capture<ValueT>, Rest...> {

  static std::string toString(ValueT value, auto... rest) {
    return "/" + std::to_string(value) + Route<Rest...>::toString(rest...);
  }

};

template<FixedString Path, typename... Rest>
struct Route<Slug<Path()>, Rest...> {       /* <------- Path() here causes compiler segfault */

  static std::string toString(auto... rest) {
    return "/" + std::string(SlugT::path) + Route<Rest...>::toString(rest...);
  }

};

template <>
struct Route<> {
  static std::string toString() {
    return "";
  }
};


using BlogRoot = Route< "blog" >;
using AllPosts = Route< "blog", "posts" >;
using SinglePost = Route< "blog", "posts", Capture<int> >;

int main() {

  // /blog
  std::cout << "blog = " << BlogRoot::toString() << std::endl;

  // /blog/posts
  std::cout << "allPosts = " << AllPosts::toString() << std::endl;

  // invalid path, compilation error
  // std::cout << "singlePost = " << SinglePost::toString() << std::endl;

  // /blog/posts/42
  std::cout << "singlePost = " << SinglePost::toString(42) << std::endl;

  return 0;
}

