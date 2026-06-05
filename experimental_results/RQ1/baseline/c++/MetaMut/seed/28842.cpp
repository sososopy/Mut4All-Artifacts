
//foo.cpp:

    #include <string>

    std::string my_global_string(100, 'x');

//main.cpp:

    #include <string>
    #include <iostream>

    extern std::string my_global_string;

    int main() {
      std::cout << my_global_string << std::endl; 
    }

