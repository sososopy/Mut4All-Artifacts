
struct Outer {
    template<class T>
    struct Inner { ~Inner(); };
};

template<>
Outer::Inner<int>::~Inner<int>() {}

