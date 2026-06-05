
#include <iostream>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

template<class ..._Attrs>
struct Test{
    using Attrs22 = tuple< _Attrs... >;
    Attrs22 attrs22;
    
    void setup(){
        int constexpr size2 = tuple_size<Attrs22>::value;
        int idsa[size2];
        
        //int idsa[tuple_size<Attrs22>::value];       // this works ok!
        
        auto fn =  [&](auto &&attr){
           idsa[0];
        };
        fn(11);        
    }
};


int main()
{
    Test<int, int, int> t;
    t.setup();  
}

