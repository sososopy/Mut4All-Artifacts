
template <class X, class... Rest> concept bool C() {   
  return true;   
}   
   
template <class... Xs>   
  requires requires(const Xs&... xs) {   
    requires C<decltype(xs)...>();   
  }   
struct A {   
};   
   
int main() {   
  return 0;   
} 
