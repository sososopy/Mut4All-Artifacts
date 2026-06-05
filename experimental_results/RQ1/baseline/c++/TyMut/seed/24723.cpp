
#include <iostream>
#include <utility>

class A : public std::pair<int,bool> {
  // NOTE here it is not possible to ommit the template id
  // EXPECTED "using std::pair::pair;"
  using std::pair<int,bool>::pair;
};

template<class T>
class B : public std::pair<T,bool> {
  // NOTE here it is not possible to ommit the template id
  // EXPECTED "using std::pair::pair;"
  using std::pair<T,bool>::pair;
};

// NOTE following examples use namespace std
using namespace std;

class C : public pair<int,bool> {
  using pair::pair;
};

template<class T>
class D : public pair<T,bool> {
  // NOTE here it is not possible to ommit the template id
  // EXPECTED "using pair::pair;"
  using pair<T,bool>::pair;
};

int main() {
  A a(5,true);
  cout << "a.first:" << a.first << endl;
  
  B<int> b(5,true);
  cout << "b.first:" << b.first << endl;
  
  C c(5,true);
  cout << "c.first:" << c.first << endl;
  
  D<int> d(5,true);
  cout << "d.first:" << d.first << endl;
  
  return 0;
}
