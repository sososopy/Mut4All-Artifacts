
struct Struct  {                                                                                                                                                                 
  virtual void virtual_func();                                                                                                                                                   
};                                                                                                                                                                               
                                                                                                                                                                                  
extern thread_local Struct& thread_local_ref;                                                                                                                                    
                                                                                                                                                                                 
bool other_func(void);                                                                                                                                                           
                                                                                                                                                                                 
bool test_func(void) {                                                                                                                                                           
  thread_local_ref.virtual_func();                                                                                                                                               
  return other_func() && test_func();                                                                                                                                            
}

