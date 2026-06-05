
module;
namespace std {
  template <class> struct initializer_list {
    int *_M_array;
    unsigned long _M_len;
  };
  struct basic_string {
    ~basic_string();
  };
}
export module repro;
struct A {
  std::basic_string data3;
};
struct V {
  V(std::initializer_list<A>);
};
struct data {
  V v{{}};
};
