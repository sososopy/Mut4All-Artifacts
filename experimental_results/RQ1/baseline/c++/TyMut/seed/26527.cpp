
template<typename _type, typename _stream>
concept streamable = requires(_stream &s, _type &&v) {
  s << static_cast<_type &&>(v);
};

struct type_a {
  template<typename _arg>
  type_a &operator<<(_arg &&) {
    // std::clog << "type_a" << std::endl;
    return *this;
  }
};

struct type_b {
  type_b &operator<<(type_a const &) {
    // std::clog << "type_b" << std::endl;
    return *this;
  }
};

struct type_c {
  type_b b;
  template<typename _arg>
  requires streamable<_arg, type_b>
  friend type_c &operator<<(type_c &c, _arg &&a) {
    // std::clog << "type_c" << std::endl;
    c.b << static_cast<_arg &&>(a);
    return c;
  }
};

int main() {
  type_a a;
  type_c c;
  a << c; // "type_a\n" (gcc gives error here)
  c << a; // "type_c\ntype_b\n"
}
