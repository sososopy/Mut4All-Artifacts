
#include <memory>

template <class T, class Deleter = std::default_delete<T>>
class unique_ptr_wrapper: public std::unique_ptr<T, Deleter>
{
public:
    using typename std::unique_ptr<T, Deleter>::unique_ptr; // typename should be forbidden here
};

int main()
{
    unique_ptr_wrapper<int> upw{new int{42}};
}

