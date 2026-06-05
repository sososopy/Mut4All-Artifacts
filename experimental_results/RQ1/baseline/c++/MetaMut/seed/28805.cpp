
template <typename T> void fun() {}                     
                                                        
template<typename T> inline constexpr bool var = false; 
                                                        
consteval bool funC() {                                 
  void (*a)() = fun<int>;                               
  void (*b)() = fun<float>;                             
  return a == b;                                        
}                                                       
                                                        
constexpr auto x = funC();                              
                                                        
consteval bool varC() {                                 
  const void* a = &var<int>;                            
  const void* b = &var<float>;                          
  return a == b;                                        
}                                                       
                                                        
constexpr auto y = varC();                              
                                                        
int main() { return 0; } 
