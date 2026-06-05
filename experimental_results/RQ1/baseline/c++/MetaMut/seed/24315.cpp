
  #pragma GCC visibility push(hidden)
  #define YYY_VIS ???
  #define XXX_VIS ???
  class YYY_VIS YYY;
  template <typename T>
  class XXX_VIS XXX {
    void func() {
      return;
    }
  };
  template class XXX<YYY *>;

