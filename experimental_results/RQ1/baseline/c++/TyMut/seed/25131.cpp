
 struct S {
     constexpr S(int & x): n(x) {}
     int & n;
 };
 constexpr S f1(int & x) { return S(x); }
 constexpr S f2(int & x) { return f1(x); }
 S f3(int & x) { return f2(x); }
 int main() {
     int n = 1;
     return f3(n).n;
 }

