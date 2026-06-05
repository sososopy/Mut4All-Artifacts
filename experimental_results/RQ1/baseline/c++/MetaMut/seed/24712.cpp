
typedef char true_type;
typedef int false_type;

struct SubObject {
    template<class V>
    void visit(V v) {
        v(1); // error here
    }
};

struct Object {
    template<class V>
    void visit(V v) {
        v(SubObject());
    }
};

struct VisitorMatcher {
    template <typename T>
    void operator()(T);
};

template <typename T>
struct hasVisitor {
    template <typename C>
    static true_type test(decltype(&C::template visit<VisitorMatcher()>));
    //template <typename C>
    //static true_type test(decltype(&C::template visit<int>));
    template <typename C> static false_type test(...);
    typedef decltype(test<T>(0)) type;
};

struct Visitor {
    template<class T>
    void operator()(T v)
    {
        typedef typename hasVisitor<T>::type type;
        // impl(v, type());
    }
};

int main()
{
    Object o;
    o.visit(Visitor());
}
