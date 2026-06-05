

#include <random>
#include <iostream>

class rng {
  public:
  std::mt19937_64 engine;
  std::uniform_real_distribution<double> distr;
  double get_val() { return distr(engine); }
  };





thread_local rng rng_instance;



int main(int argc, char** argv)
  {
  std::cout << "val: " << rng_instance.get_val() << std::endl;
  return 0;
  }
