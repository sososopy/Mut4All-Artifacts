
 #include <cstddef>
 template<typename> struct Reference {};
 template<typename> struct S2 {
   static void * operator new(std::size_t);
   static void operator delete(void *);
   S2();
 };
 void f() { new S2<Reference<int>>; }

