
#include <cilk/cilk.h>                                                           
#include <cilk/cilk_api.h>                                                       
                                                                                 
struct A {                                                                       
  A() = default;                                                                 
                                                                                 
  A(const A&) {} // uncomment and it works                                       
                                                                                 
  // A(A &&) {} // broken for move constructors also                              
};                                                                               
                                                                                 
A f() {                                                                          
  return A{};                                                                    
}                                                                                
                                                                                 
void compute_cilk() {                                                            
  auto v1 = cilk_spawn f();                                                      
  auto v2 = f();                                                                 
  cilk_sync;                                                                     
}                                                                                
                                                                                 
int main() {                                                                     
  compute_cilk();                                                                
  return 0;                                                                      
} 
