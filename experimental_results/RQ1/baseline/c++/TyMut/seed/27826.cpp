
struct X {
  template<typename F, F Method, typename ... Args>
  auto Decorator(Args&&...args) {
    // std::cout << "decorator\n";
    (this->*Method)(&args...);
  }

  void bar() {
    constexpr auto m_ptr = &X::barImpl;
    Decorator<decltype(m_ptr), m_ptr>();
  }

 
  void barImpl() {
    // std::cout << "bar\n";
  }
 
};
