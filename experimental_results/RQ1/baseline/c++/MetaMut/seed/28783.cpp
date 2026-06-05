
#include <iostream>

class MyString {
public:
  MyString(const char* s = "") {
    std::cout << "ctor" << std::endl;
  }
  ~MyString() { 
    std::cout << "dtor" << std::endl;
  }
  MyString(const MyString& s) {
    std::cout << "copy ctor" << std::endl;
  }
  MyString& operator=(const MyString& s) {
    std::cout << "operator=" << std::endl;
    return *this;
  }
};

int main() {
  MyString s1 = "Hello";
  std::cout << __LINE__ << std::endl;
}

