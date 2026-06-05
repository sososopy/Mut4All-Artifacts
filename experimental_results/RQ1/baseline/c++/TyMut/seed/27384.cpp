
  template<typename>
  class A
  {
  };

  class B
  {
      template<typename, typename>
      friend class ::A;
  };

  int main()
  {
  }

