
#include <map>
 
template <typename Type>
class VecBase
{
    typedef __attribute__((aligned(8))) Type value_data;
};
 
typedef std::map<double, double> test_passed;
 
struct S1
{
  inline void f1(VecBase<const double> p1) {}
};
 
typedef std::map<double, double> test_failed;
 
int main()
{
  return 0;
}
