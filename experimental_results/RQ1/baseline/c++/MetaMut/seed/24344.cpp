
#include <cstddef>

template<int Num>
class N {
    static constexpr int num = Num;
};

template<class T> using NDef = N<offsetof(T, T::m)>;
template<class T> using TDef = NDef<T>;


