
class Base {
protected:
  template<class... TArgs>
  Base(TArgs...) {}

  // Uncomment to workaround                                                                                                                                                     
  //Base() {}                                                                                                                                                                    
};

class Class
  : public Base {
public:
  template<class... TArgs>
  Class(TArgs... args) : Base { args... } {}

  // Another workaround:                                                                                                                                                         
  //Class() {}                                                                                                                                                                   
};

void test() {
  Class{};
}

