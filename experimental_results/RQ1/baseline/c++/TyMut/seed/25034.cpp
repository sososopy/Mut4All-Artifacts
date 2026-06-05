
class A {                                                                          
  int table_clear;                                                                 
};                                                                                 
                                                                                   
template <typename T>                                                              
class B : T {                                                                      
  B() { this->A::table_clear; }                                                    
};                                                                                 
                                                                                   
int main() {} 
