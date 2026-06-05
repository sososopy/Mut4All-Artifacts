
class a {
  template <typename b> a(b());
  template <typename b> a(b(__attribute__((fastcall)) c)());
};
