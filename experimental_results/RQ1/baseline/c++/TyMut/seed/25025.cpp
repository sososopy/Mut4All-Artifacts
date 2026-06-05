
template <class T>
class rv: public T
{
   rv();
   ~rv() throw();
   rv(rv const&);
   void operator=(rv const&);
} __attribute__((__may_alias__));

class A
{
  A(A &);
  A& operator=(A &);
public:
  explicit A(rv<A>& safetyCamera);
  A& operator=(rv<A>& safetyCamera);
  operator const rv<A>&() const { return *static_cast<const rv<A>*>(this); }
};

A::A(rv<A>&) {}
A& A::operator=(rv<A>&) { return *this; }


