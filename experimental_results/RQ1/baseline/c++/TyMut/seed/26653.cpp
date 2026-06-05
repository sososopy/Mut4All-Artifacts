#include <typeinfo>

class Interface {
public:
  virtual int size() const = 0;
};

template <class T>
class Vector {
public:
  void resize(unsigned n, T val = T()); //C++03 style resize
//  void resize(unsigned n, const T& val); //C++11 style resize
private:
  T* data;
};

int main()
{
  typeid(Interface).name();
  typeid(Interface()).name(); //GCC 4.9.2 and VS2013: doesn't compile

  typedef Interface* PtrInterf;
  PtrInterf p1 = 0;
  typeid(PtrInterf).name();
  typeid(PtrInterf()).name();
  typeid(p1).name();

  //typeid(Vector<Interface>).name(); //GCC 4.9.2 and icc 13.0.1: doesn't compile
  typeid(Vector<Interface>()).name();

  typedef Vector<Interface>* PtrVecInterf;
  PtrVecInterf p2 = 0;
  typeid(PtrVecInterf).name(); //GCC 4.9.2: doesn't compile
  typeid(PtrVecInterf()).name();
  typeid(p2).name(); //GCC 4.9.2: doesn't compile

  return 0;
}

