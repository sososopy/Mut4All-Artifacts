
#include <tuple>
#include <utility>
using namespace std;

template<class ..._Attrs>
class VertexBuffer
{
    static constexpr const bool ownIndices = sizeof...(_Attrs) == 0;
    
    void test(){
        static_asssert(ownIndices, "Link to owned indices!!");        
    }
}


int main(){}

