

struct X {};
template<typename T> struct Q { static X x; };
template<> X Q<int>::x{};


template<typename T> X Q<T>::x{};


template<> X Q<int>::x[]{};
template<> int Q<int>::x{};
template<> int Q<int>::x{1};
template<typename T> X Q<T>::x{};

