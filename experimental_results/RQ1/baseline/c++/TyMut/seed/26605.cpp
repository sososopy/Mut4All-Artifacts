
template <class T>
inline void AssertMoveablePtr(T, T) {}

template <class T>
inline void AssertMoveable0(T *t) { AssertMoveablePtr(&**t, *t); }
// COMPILATION ERROR HERE MEANS TYPE T WAS NOT MARKED AS Moveable

template <class T>
struct Moveable_ {
        friend void AssertMoveable0(T *) {}
};

template <class T>
inline void AssertMoveable(T *t = 0) { if(t) AssertMoveable0(t); }

#if defined(COMPILER_MSC) || defined(COMPILER_GCC) && (__GNUC__ < 4 || __GNUC_MINOR__ < 1)
        #define NTL_MOVEABLE(T) inline void AssertMoveable0(T *) {}
#else
        #define NTL_MOVEABLE(T) template<> inline void AssertMoveable<T>(T *) {}
#endif


NTL_MOVEABLE(bool)
NTL_MOVEABLE(char)
NTL_MOVEABLE(signed char)
NTL_MOVEABLE(unsigned char)
NTL_MOVEABLE(short)
NTL_MOVEABLE(unsigned short)
NTL_MOVEABLE(int)
NTL_MOVEABLE(unsigned int)
NTL_MOVEABLE(long)
NTL_MOVEABLE(unsigned long)
NTL_MOVEABLE(int64)
NTL_MOVEABLE(uint64)
NTL_MOVEABLE(float)
NTL_MOVEABLE(double)
NTL_MOVEABLE(void *)
NTL_MOVEABLE(const void *)
