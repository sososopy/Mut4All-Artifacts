
 #include <iostream>
 struct S1 {
     consteval S1() { i = 1; }
     int i = 0;
 };
 struct S2 {
     constexpr S2() { i = 1; }
     int i = 0;
 };
 S1 const s1a;
 constexpr S1 s1b;
 S2 const s2;
 int main() { std::cout << s1a.i << ' ' << s1b.i << ' ' << s2.i << '\n'; }

