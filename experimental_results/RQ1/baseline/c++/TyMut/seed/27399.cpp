
#include <type_traits>

  struct A
    {
    int i:31;
    };

  A a;

  //I am not sure the standard require that (a.i) and ++a.i have the same type.
  static_assert(!std::is_same_v <decltype ((a.i)), decltype(++a.i)>);
                          //      int&               int:31&

  //but I do believe that decltype(a.i+=1) and decltype(++a.i) shall have the same type
  static_assert(!std::is_same_v <decltype ((a.i+=1)), decltype(++a.i)>);
                         //                int&            int:31&
