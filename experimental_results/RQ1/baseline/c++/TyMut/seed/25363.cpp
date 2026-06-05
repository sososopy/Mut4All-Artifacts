
struct A
{
//    A();
    ~A(); // NEEDED to produce ICE
};

#ifdef FIRST_CASE
A a (int) { throw ""; return {};}
A b (int) { throw ""; return {};}

A c (int) { throw ""; return {};}
A d (int) { throw ""; return {};}
A e (int) { throw ""; return {};}

#else // second case

A a (int) { throw 1;}
A b (int) { throw 1;}

A c (int) { throw 1;}
A d (int) { throw 1;}
A e (int) { throw 1;}

#endif
//EOF

