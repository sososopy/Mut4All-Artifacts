
#include <iostream>

int main(int, char**) 
{ 
  unsigned int mt[624]; 
  std::cout << " Array status mt[] = " << std::endl;
  for (size_t i=0; i<624; i+=5) {
    std::cout << i << " " << mt[i]   << " " << mt[i+1] << " " << mt[i+2] << " " 
              << mt[i+3] << " " << mt[i+4] << std::endl;
  }
  std::cout << "----------------------------------------" << std::endl;

  return 0; 

}
