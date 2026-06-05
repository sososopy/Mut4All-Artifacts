
struct A {                                                                         
  ~A() {}                                                                          
};                                                                                 
                                                                                   
A f() {                                                                            
  return {};                                                                       
}                                                                                  
                                                                                   
int main() {                                                                       
  _Cilk_spawn f();                                                                 
  return 0;                                                                        
}    
