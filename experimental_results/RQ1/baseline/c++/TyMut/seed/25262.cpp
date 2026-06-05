

class A; 

typedef void (A::*fp) (void);

struct B
{
  int n; 
  fp f;
};
 
struct C
{
  const C *c;
  const B b; 
};

class D 
{
public: 
  static B e;
  static C m;
};

class E : public D 
{
public: 
  void foo (); 
};

// OK: B D::e = { 0, (fp) &E::foo }; 
const B D::e = { 0, (fp) &E::foo }; 

C D::m = { &D::m, E::e };
