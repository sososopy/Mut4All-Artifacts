
#include <type_traits>

class Foo {
public:
// With this, the DestructorReturnType below becomes "void"
//    ~Foo () {} 
};

// … unless I specify the user-defined destructor above, in which case it is "void"
using DestructorReturnType = decltype(std::declval<Foo>().~Foo());

template<class T>
class TD;

// Says: aggregate 'TD<void&> t' has incomplete type and cannot be defined
TD<DestructorReturnType> t;
