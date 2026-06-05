
struct A {                                                                         
  A() = default;                                                                   
  A& operator=(const A&) { 
      return *this;
  }                                                                                
};                                                                                 
                                                                                   
A f() { return {}; }                                                               
                                                                                   
void g() {                                                                         
  A a;                                                                             
  a = _Cilk_spawn f();                                                             
}
