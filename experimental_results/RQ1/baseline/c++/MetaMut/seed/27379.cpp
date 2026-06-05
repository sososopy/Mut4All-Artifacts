
namespace std {
  template<typename T> struct initializer_list {
    const T *ptr;
    decltype(sizeof 0) n;
  };
  auto a = {};
}


