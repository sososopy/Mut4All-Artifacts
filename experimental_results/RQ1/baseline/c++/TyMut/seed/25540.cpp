
class Adaptee { };



class Adapter {
   template<typename T>
   class Method {
      static void Get(const T *);
   }

   template<typename T>
   static void Adapt(const T *v) {
      Method<T>::Get(v);
   }
};


#include <Adaptee.h>
#include <Adapter.h>

template<>
void Adapter::Method<Adaptee>::Get(const Adaptee *) { }


#include <Adaptee.h>
#include <Adapter.h>

int
main()
{
   Adaptee a;
   Adapter::Adapt(&a);
}

