
  #include <string_view>
  
  constexpr std::string_view f() { return "hello"; }
  
  static constexpr std::string_view g() {
      auto x { f() };
      return x.substr(1, 3);
  } 
  
  int foo() { return g().length(); }

