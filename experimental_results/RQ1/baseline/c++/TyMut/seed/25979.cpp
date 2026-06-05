template <class Elt, class Tr>
struct my_string {
  static const unsigned npos = ~0U;
};

template <unsigned bits>
struct my_bitset {

  my_bitset() { }

  template <class Elt, class Tr>
  explicit my_bitset(unsigned s = my_string<Elt, Tr>::npos) { }

};

my_bitset<2> inst;
