
template <class>
using A = int *;
template <class T, template <class> class U>
struct B { typedef U <typename T::type> type; };
struct C { typedef int *type; };
template <class T>
struct D {
  D <C> foo () { return D <C> (); }
  template <template <class> class U>
  U <typename T::type> bar ();
};
struct E { typedef int type; };
D <B <E, A>> d;

