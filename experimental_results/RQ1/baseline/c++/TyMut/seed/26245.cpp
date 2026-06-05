
#include <iostream>
struct TMyStruct
   { };
struct TByteArrayList
   {
      TByteArrayList(const TByteArrayList &)
         { i = 12; std::cout << "copy ctor " << this << std::endl; }
      TByteArrayList &operator=(const TByteArrayList &)
         { std::cout << "operator = " << this << std::endl; return *this; }
      TByteArrayList()
         { i = 12; std::cout << "ctor " << this << std::endl; }
      ~TByteArrayList()
         { std::cout << "dtor " << this << std::endl; }

      TByteArrayList &operator<<(const TMyStruct &)
         { std::cout << "<< " << this << std::endl; return *this; }

      int i;
   };

int main()
   {
      const TByteArrayList &List = TByteArrayList() << TMyStruct();

      std::cout << "Here some use of List: " << List.i << " address " << &List << std::endl;

      return 0;
   }

