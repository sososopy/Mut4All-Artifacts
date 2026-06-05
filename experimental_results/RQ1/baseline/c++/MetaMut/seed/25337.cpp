

class B {
protected:
  void fprot();
  int  iprot;
private:
  void fpriv();
  int  ipriv;
};

A normal non-templated derived class appears to honour the visibility of base class members in the way that I'd expect:

class A : public B {
public:
  void f() {
    void (B::*f)();
    int i;
    //f = &B::fprot; - protected in this context
    //f = &B::fpriv; - private in this context
    f = &A::fprot;  // OK - access to protected base member via public derivation
    //f = &A::fpriv; - private in this context
    fprot();        // OK - access to protected base member via public derivation
    //fpriv();       - private in this context
    i = iprot;      // OK - access to protected base member via public derivation
    //i = ipriv;     - private in this context
  }
};

