
     template <typename Sig, Sig& S> struct OpF;

     template <typename TR, typename ... Ts, TR (&f)(Ts...)> struct
 OpF<TR (Ts...), f> { };

     int foo(int x) { return 0; }

     OpF<int (int), foo> f;

