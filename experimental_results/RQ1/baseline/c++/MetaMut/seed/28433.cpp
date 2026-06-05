
template <typename T>                                                                                                                                                                                                                                           
class myclass {                                                                                                                                                                                                                                                    
public:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
 
  void func(const T&) requires true
  {
     std::cout << "true";
  }
 
  void func(const T&) requires false
  {
     std::cout << "false";
  }
};                                                                                                                                                                                                                                                              


int main(){                                                                                                                                                                                                                                
                                                                                                                                                                                                                                             
  myclass<int> obj;                                                                                                                                                                                                                             
  auto mylambda = std::bind(&myclass<int>::func, &obj, 5); //accepted by clang rejected by gcc
  mylambda();                                                                                                                                                                                                                                                     

} 
