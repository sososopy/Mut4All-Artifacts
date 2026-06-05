

  #include <initializer_list>

  struct S {
    int value;
  };

  int F(S);
  char* F(std::initializer_list<int>);

  char* p = F({.value = 0});

