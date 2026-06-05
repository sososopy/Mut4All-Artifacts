
struct A{A()=default;A(A&)=default;};
template<typename B>struct C:B{C()=default;
#if 1
C(const C&)=default;//Fails
#else
C(const C&c):B(c){};//Works
#endif
};
C<A> d;
int main(){}
