
#include <iostream>
 
using namespace std;
 
struct far {
    struct boo {
        boo () {
            cerr << "bar::boo" << endl;
        }
        int i = 42;
    };
     
    static void baz() {
        cerr << far::FOO.i << endl;
    }
     
    static thread_local boo FOO;
};

thread_local typename far::boo far::FOO;
 
int main() {
    far f;
    cerr << f.FOO.i << endl;
    cerr << far::FOO.i << endl;
    return 0;
}

