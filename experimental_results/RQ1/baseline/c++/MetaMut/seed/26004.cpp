
#include <functional>
struct Test {
  template<typename... Args> inline void triggerTest (Args&&... fargs) { } 
};
struct TestPickled : Test {  
  template<typename... Args> void triggerTest (Args&&... fargs) { 
    std::bind([=](Args... as) { // same problem when passing argument pack to lambda instead of std::bind
      Test::triggerTest(as...);
      // this->_Test::triggerTest(as...);
      // ^^^^^^^ THIS WORKAROUNDS THE PROBLEM
    }, fargs...);		
  }
};
int main()
{
  TestPickled test;
  test.triggerTest();
  return 0;
}
