

#include <cassert>                                                                                                    
#include <cstdint>                                                                                                    
                                                                                                                      
constexpr std::int64_t                                                                                                
cx_convert(char const * const s, std::size_t const length)                                                            
{                                                                                                                     
  std::size_t i = 0;                                                                                                  
  std::int64_t whole = 0;                                                                                             
  while (i < length) {                                                                                                
    whole *= 10;                                                                                                      
    whole += s[i++] - '0';
    // Replace line above with this and expected value is returned
    // whole += s[i] - '0'; i++;
  }                                                                                                                   
  return whole;                                                                                                       
}                                                                                                                     
                                                                                                                      
int main() {                                                                                                          
  // Passes!!!!!
  static_assert(cx_convert("123", 3) == -28);

  // Fails; cx_convert returns -28 as above
  static_assert(cx_convert("123", 3) == 123);
           
  // Passes at runtime if second static_assert is commented out.
  assert(cx_convert("123", 3) == 123);
}                               

