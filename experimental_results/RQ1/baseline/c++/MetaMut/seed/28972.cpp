
// File: new_crash.cpp
#include <cstdint>
#include <limits>
#include <new>

class MemAlloc {
  public:
    MemAlloc() {}
    void* operator new[](size_t sz, const std::nothrow_t& nt) {
        return ::operator new(sz, nt);
    }
};

template <typename T>
class VarArray : public MemAlloc
{
  public:
    VarArray() {}
    ~VarArray(){}
    static T*
    allocbuf(uint32_t nelems) {
        return new(std::nothrow) T[static_cast<size_t>(nelems)];
    }
    void* operator new[](size_t sz, const std::nothrow_t& nt) {
        return MemAlloc::operator new[](sz, nt);
    }
};

class MyType {
  public:
    void* operator new[](size_t sz, const std::nothrow_t& nt) {
        return MemAlloc::operator new[](sz, nt);
    }
    uint32_t m_id;
    VarArray<int> m_int_seq;
};

class MyTypeList : private VarArray<MyType>
{
  public:
    using VarArray<MyType>::allocbuf;
    using VarArray<MyType>::operator new[];
};

int main() 
{
    const uint32_t max_uint32t = std::numeric_limits<uint32_t>::max();
    MyType *type_list = MyTypeList::allocbuf(max_uint32t);

    if (type_list) {
        delete[] type_list;
    }        

    return 0;
}
