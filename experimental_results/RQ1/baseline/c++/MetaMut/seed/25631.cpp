
   template <template <typename...> class F, typename P> struct apply;

   template <
       template <typename...> class F,
       template <typename...> class L, typename... T
   > struct apply <F, L <T...> > { typedef L <F <T>...> type; };

   template <typename T> using map = const T;
   template <typename> struct A { };
   template <typename> void dummy();

   int main() { dummy <apply <map, A <int> >::type>(); }
