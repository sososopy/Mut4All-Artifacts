
  template <typename A>
  struct foo
  {
  };
  
  // template specialization
  template<>
  struct foo<int>
  {
      static constexpr int code = 42;
      unsigned int bar = static_cast<unsigned int>(code);
      // ICE at static_cast during instantiation. --^
  };
  
  // instantiation
  foo<int> a;

