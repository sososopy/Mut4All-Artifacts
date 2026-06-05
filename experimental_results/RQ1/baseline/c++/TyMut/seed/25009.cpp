


#include <iostream>     // std::cout
#include <new>          // ::operator new
#include <vector>
#include <tuple>
#include <type_traits>

using namespace std;

struct Empty{};

    
template<class T>   /* <-- Because of Template */
struct Data{
    int x;
    float y;
    
    int properties_parcel4[10];
    
    Empty j = [&](){
		int i = 10;
                properties_parcel4[0] = i;
		return Empty(); 
    }();    
};

int main () {
    Data<int> k;
    
  return 0;
}
