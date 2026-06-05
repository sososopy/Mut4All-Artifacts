

#include <iostream>
using namespace std;

  
struct testclass
{
  int data;
  inline operator const int&() const
  {
    return data; 
  }
  
  testclass& operator = (const int& in)
  {
    data = in;
    return *this;
  }
};


template <typename A, typename B> auto mymax14(const A& a, const B& b)
{
  return std::max((int)a, (int)b);
}

template <typename A, typename B> auto mymax11(const A& a, const B& b) -> decltype(std::max((int)a, (int)b))
{
  return std::max((int)a, (int)b);
}



int main()
{

  testclass d;

  d = 1;
  cout << d.data << endl;   // ok, d.data==1

  d = mymax14(d, 2);
  cout << d.data << endl;   // ok, d.data==2

  d = mymax11(d, 2);
  cout << d.data << endl;   // bad: d.data == some random number.

  return 0;
}
