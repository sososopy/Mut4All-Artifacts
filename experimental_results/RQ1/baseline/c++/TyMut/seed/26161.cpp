

class A { };
class B: virtual A { };
class C: virtual B { };

class D: C
{
   void operator= (D &);
};


