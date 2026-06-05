
#include <utility>
#include <concepts>
#include <future>

template<typename T>
concept my_concept = std::regular<T>;

template <typename T, typename runtime_t> 
class wrapper
{
 T d;
 runtime_t& x;
public:
 wrapper(T&& data, runtime_t& runtime) : 
    d(std::move(data)),
    x(runtime) {}
T& fut()
{
    return d;
}
runtime_t& runtime()
{
    return x;
}
};


template<my_concept field_element>
//template<typename field_element>  // replacing the above line by this line fixes the compilation error on GGC-trunk
class test {
public:
    friend auto fun(wrapper<std::shared_future<field_element>, test<field_element>>& a,
                        wrapper<std::shared_future<field_element>, test<field_element>>& b)
        -> wrapper<std::shared_future<field_element>, test<field_element>>
    {
        return wrapper<std::shared_future<field_element>, test<field_element>> (   
            std::async(std::launch::deferred, [a,b]() mutable { return a.fut().get() + b.fut().get(); }),
            a.runtime() 
            );
    }
};

int main(){
    static_assert(std::regular<int>);

    test<int> t;
    std::shared_future<int> f1 = std::async(std::launch::deferred, []() { return 42; });
    std::shared_future<int> f2 = std::async(std::launch::deferred, []() { return 42; });

    wrapper<std::shared_future<int>, test<int>> a(std::move(f1),t); 
    wrapper<std::shared_future<int>, test<int>> b(std::move(f2),t); 

    fun(a, b);
    
}
