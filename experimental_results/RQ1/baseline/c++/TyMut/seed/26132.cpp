
  template<typename T> constexpr int f(T);          
  int k = f(0);
  template<typename T> constexpr int f(T) { return 0; }