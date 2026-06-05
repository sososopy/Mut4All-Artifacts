
template <int I>                                                                 
struct A{                                                                        
  constexpr operator int() { return I; }                                         
};                                                                               
                                                                                 
template <int I, class T>                                                        
using my_t = A<I>;                                                               
                                                                                 
template<int I, class T>                                                         
class B {                                                                        
 public:                                                                         
  static constexpr int x = my_t<I, T>{};                                         
};                                                                               
                                                                                 
int main() {                                                                     
  return 0;                                                                      
} 
