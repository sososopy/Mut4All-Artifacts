
class a {
public:
  int b;
};
class c : a {
  auto m_fn1() -> decltype(b);
};

