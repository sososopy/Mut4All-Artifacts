
 #include <type_traits>
 
 struct A{};
 
 template <auto a> 
 void f() { 
     static_assert(std::is_same_v<decltype(a), A>);
 }
 
 template <auto const a> 
 void g() { 
     static_assert(std::is_same_v<decltype(a), A>);
 }
 
 int main() {
     constexpr A a{};
     f<a>();
     g<a>();
 }

