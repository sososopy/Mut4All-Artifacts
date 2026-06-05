
int a; 

class A 
{ 
public: 
  virtual ~A () {} 
};

class B1 : public A {};
class B2 : public A {};

class C 
{ 
public: 
  virtual ~C () {} 
};

class D : public C, public B2 
{ 
public: 
  virtual ~D () {} 
};

class E : public B1, virtual public D
{ 
public: 
  virtual ~E () {  a = 0; } 
};

int
main ()
{
  a = 1;
  delete (D *) (B1 *) new E;  // Does this lead to undefined behavior?  
  if (a) 
    return 1; 
  return 0; 
}
