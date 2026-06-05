
struct B {                                                                       
};                                                                               
                                                                                 
B f() { return {}; }                                                             
                                                                                 
void g() {                                                                       
  B b;                                                                           
  b = _Cilk_spawn f();                                                           
}                                                                                
                                                                                 
int main() {                                                                     
  g();                                                                           
  return 0;                                                                      
}
