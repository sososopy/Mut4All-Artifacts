
  template <size_t alignment = GPUCA_BUFFER_ALIGNMENT, class S>
  static inline S* getPointerWithAlignment(char*& basePtr, size_t nEntries = 1)
  {
    auto t = reinterpret_cast<size_t>(basePtr);
    auto t1= getPointerWithAlignment<alignment, S>(reinterpret_cast<size_t&>(basePtr), nEntries);
    basePtr = reinterpret_cast<decltype(basePtr)>(t);
    return t1;
  }

  template <size_t alignment = GPUCA_BUFFER_ALIGNMENT, class T, class S>
  static inline void computePointerWithAlignment(T*& basePtr, S*& objPtr, size_t nEntries = 1)
  {
    size_t t = reinterpret_cast<size_t>(basePtr);
    objPtr = getPointerWithAlignment<alignment, S>(t, nEntries);
    basePtr = reinterpret_cast<T*>(t);
  }
