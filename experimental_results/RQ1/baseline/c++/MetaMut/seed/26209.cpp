class A
{
    virtual int String ();
};

class F: public A { };

template < typename V > class G
{
    V value;
};

class D
{
    template < int N > void Verify() {
      G<A>* x = 0;
      F& name = x->value;
      name.String();
    }
};
