

template <typename> class k;

struct d {
  ...
  void ck(k<int> const &);
  void ck(int);
};

template <typename> class k {
  template <typename> friend class ca;
};

  auto e::operator+=(e &&cl) -> e & {
    for (auto bt : cl)
      ck(bt);
    return *this;
  }

template <typename> class ca {};
ca<int> d;


