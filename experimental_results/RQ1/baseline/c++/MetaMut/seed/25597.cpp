

template<class T> struct Loop { Loop<T*> operator->(); };
Loop<int> i, j = i->something;

