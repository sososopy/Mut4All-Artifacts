

    #include <iostream>
    #include <typeinfo>
    #include <cxxabi.h>
     
    struct A{};
    template<class,class...> struct h{};
    template<class> struct hh{};
    int main() {
    typedef h<A,hh<int>> hx;
    const char* name = typeid(hx).name();
    std::cout << name << "\n";
    char db[4096];
    size_t size = 4096;
    int st;
    abi::__cxa_demangle(name,db,&size,&st);
    std::cout << db << "\n";
    // now what I think the symbol should look like (exchange
    abi::__cxa_demangle("1hI1AJ2hhIiEEE",db,&size,&st);
    std::cout << db << "\n";
    return 0;
    }
