
#include <cilk/cilk.h>                                                             
#include <cilk/cilk_api.h>                                                         
#include <vector>                                                                  
#include <random>                                                                  
#include <cmath>                                                                   
                                                                                   
const int N = 100'000'000;                                                         
                                                                                   
auto f() {                                                                         
  std::vector<double> v(N);                                                        
  auto rng = std::mt19937{std::random_device{}()};                                 
  std::uniform_real_distribution<double> dist(0, 1);                               
  for (int i = 0; i < N; ++i) v[i] = std::log(std::sqrt(dist(rng)));               
  return v;                                                                        
}                                                                                  
                                                                                   
void compute_cilk() {                                                              
  auto v1 = cilk_spawn f();                                                        
  auto v2 = f();                                                                   
  cilk_sync;                                                                       
  do_not_optimize_away(v1.data());                                                 
  do_not_optimize_away(v2.data());                                                 
}                                                                                  
                                                                                   
int main() {                                                                       
  compute_cilk();                                                                  
  return 0;                                                                        
}  
