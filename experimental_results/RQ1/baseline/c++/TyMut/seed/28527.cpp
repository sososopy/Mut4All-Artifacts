

#include <memory>
using namespace std;

typedef int Ary0[0];
int main(){
	unique_ptr<Ary0> ptr;
	ptr.reset(new Ary0[0]);
	return 0;
}



