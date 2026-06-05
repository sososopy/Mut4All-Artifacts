
#include <iostream>
#include <type_traits>

template <typename V>
struct subclass {
    template <typename V2 = V, 
              typename = typename std::enable_if<!std::is_same<V2,int>::value>::type> 
explicit operator int(){return 1;};

    operator V(){return 2.;}
    friend std::ostream& operator<<(std::ostream&out, const subclass p){out << 3; return out;}
};

int main()
{
    subclass<int> aaaa;
    subclass<double> bbbb;

    std::cout << aaaa << std::endl; // This causes a crash
    std::cout << bbbb << std::endl;
    std::cout << int(aaaa) << std::endl;
    std::cout << int(bbbb) << std::endl;
}

