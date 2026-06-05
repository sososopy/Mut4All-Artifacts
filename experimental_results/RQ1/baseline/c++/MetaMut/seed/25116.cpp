

template<bool, class> struct enable_if {};
template<class T> struct enable_if<true, T> { using type = T; };

template <typename X>
struct foo
{
     template <typename R>
     using meow = typename enable_if<sizeof(X) == 0, R>::type;

     template <typename R = int>       // 1
     meow<R> bar () = delete;

     int bar ()
     {
        meow<int> i;          // 2
        return 0;             // 3
     }
};

int j = foo<long>().bar();

