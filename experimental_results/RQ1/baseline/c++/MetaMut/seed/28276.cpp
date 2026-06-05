
#include <vector>
template<typename T>
struct Adapter: std::vector<T>{
    using std::vector<T>::vector;

};
template<typename T>
Adapter(std::initializer_list<T>) -> Adapter<T>;

int main(){
    Adapter x{1,2,3};
}
