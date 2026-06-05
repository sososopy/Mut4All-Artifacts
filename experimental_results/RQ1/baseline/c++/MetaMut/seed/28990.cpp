
  template<typename... Args>
  struct A {
   template<auto N>
   struct B {};
  };
  
  template<>
  template<typename... Args>
  struct A<Args...>::B<sizeof...(Args)> {};

