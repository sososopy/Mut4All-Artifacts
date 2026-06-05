

  namespace std { struct type_info {}; }
  struct A {};
  auto x = typeid(void(A::*)() const);

