class b {
  void operator++() = delete;
  template <class> void operator++(int) {
    b bb;
    ++bb;
  }
};