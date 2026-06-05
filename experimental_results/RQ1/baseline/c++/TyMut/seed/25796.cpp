
typedef decltype(nullptr) NullPtrType;
template <NullPtrType> void foo();
template <> void foo<nullptr>() { }
