
class C { 
  template <class T> 
  C tpl_mem(T *) { 
    return 
  } 
  void mem(int *p) { 
    tpl_mem(p); 
  } 
}; 
class C2 { 
  void f(); 
}; 
void C2::f() {} 
