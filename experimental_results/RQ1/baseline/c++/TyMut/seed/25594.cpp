
#include <iostream>
using namespace std;
template <class T> class foo;
template <class T> ostream & operator<<(ostream& o, const foo<T>& l);
template <class T> class foo  {
    friend ostream& operator<< <T> (ostream& o, const foo<T>& l);
};
class bar;
foo<bar> fb;
class bar { virtual void baz()=0; };
