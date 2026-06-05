
#include <atomic>
#include <type_traits>

struct A
{                                                                                                                                                                                                          
    int i_;                                                                                                                                                                                                
                                                                                                                                                                                                           
    A(int i) : i_(i) {}                                                                                                                                                                                    
};                                                                                                                                                                                                         
                                                                                                                                                                                                           
static_assert(std::is_trivially_copyable<A>::value, "");                                                                                                                                                   
                                                                                                                                                                                                           
int                                                                                                                                                                                                        
main()                                                                                                                                                                                                     
{                                                                                                                                                                                                          
    std::atomic<A> q(A(1));                                                                                                                                                                                
}                                                                                                                                                                                                          
