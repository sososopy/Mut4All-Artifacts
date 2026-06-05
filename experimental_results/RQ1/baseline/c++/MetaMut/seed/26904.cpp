#include <iostream>
    #include <string>
    #include <vector>
    
    template<typename T>
      std::vector<T> split (T in, typename T::value_type delim);
    // … see below
    
    void choke (std::string foo)
    {
      for (std::string foo : split (foo, ' '))
      {
        std::cout << foo << std::endl;
      }
    }
    
    int main (int, char**)
    {
      choke ("foo bar baz");
      return 0;
    }

