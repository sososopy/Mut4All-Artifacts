
namespace std {                                                                                  
template <typename... T>                                                                         
void swap(std::tuple<T&...> lhs, std::tuple<T&...> rhs) noexcept                                 
{              
  std::cout<<"ADL works!"<<std::endl;                                                                                   
  std::tuple<T...> tmp = lhs;                                                                  
  lhs = rhs;                                                                                   
  rhs = tmp;                                                                                   }

From my tests the swap function being used when I include the above code behaves as:
auto tmp = lhs;
lhs = rhs;
rhs = lhs; 
