

class A {                        };
class D;
template<typename > using UniquePtr = A;
struct C {
    typedef UniquePtr<D> SingleObject;
};
template<typename , typename... Args> C::SingleObject MakeUnique(Args... aArgs) {
    D(aArgs...);
}
class B {                        };
template<typename AllocPolicy> class BufferList {
public:
    BufferList(unsigned, unsigned, unsigned, AllocPolicy = AllocPolicy());
};
class D : BufferList<B> {
    BufferList::BufferList;
};
UniquePtr<D> setCloneBuffer_impl_buf = MakeUnique<D>(0, 0, 0);
