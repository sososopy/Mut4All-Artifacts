struct W { 
  constexpr explicit operator bool() { return true; };
};

struct U {
  explicit(W()) U(int);
};

