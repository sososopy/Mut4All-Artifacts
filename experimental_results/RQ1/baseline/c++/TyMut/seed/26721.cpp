
#include <iterator>                                                              
#include <iostream>                                                              
                                                                                 
template <class Iter>                                                            
concept bool Iterator =                                                          
    requires(Iter i, typename std::iterator_traits<Iter>::value_type val,        
             typename std::iterator_traits<Iter>::reference ref) {               
  ++i;                                                                                                                                  
};                                                                               
                                                                                 
int main() {                                                                     
  std::cout << Iterator<int*> << "\n"; // prints 1                               
  std::cout << Iterator<int> << "\n"; // prints 1                                
  return 0;                                                                      
}
