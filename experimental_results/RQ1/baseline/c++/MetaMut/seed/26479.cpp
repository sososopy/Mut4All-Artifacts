
   int f1(int);
 
   template <auto N>
   struct A {
    using F = int(int);
    operator F*() requires N { return f1; }
   };
 
   int i = A<true>{}(0);  // Fine
   int j = A<false>{}(0); // Crash

