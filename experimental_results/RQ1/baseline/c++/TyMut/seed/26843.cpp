
#include <iostream>

struct blah {
  inline double setval(unsigned int& x) const
    {
    x = 123;
    return 456.0;
    }
  };


int
main(int argc, char** argv) {
  blah blah_instance;
  
  unsigned int val = 9999;
  
  std::cout << blah_instance.setval(val) << "  val: " << val << std::endl;
  std::cout << blah_instance.setval(val) << "  val: " << val << std::endl;
  
  return 0;
  }

