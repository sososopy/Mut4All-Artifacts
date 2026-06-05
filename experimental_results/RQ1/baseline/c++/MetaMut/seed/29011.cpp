
  template<typename>
  struct A {
   template<typename...>
   struct B;
  };
  
  template<typename A_t>
  template<typename B_t>
  struct A<A_t>::B<B_t> {};
  
  template<typename A_t>
  template<typename B_t>
  requires requires {
   typename B_t;
  }
  struct A<A_t>::B<B_t> {};

