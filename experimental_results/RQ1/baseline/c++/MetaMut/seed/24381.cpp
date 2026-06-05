
#include <vector>

struct foobar {
    int a;
};

int main(){
    std::vector<foobar> foo;
    //Should fail: foobar has no constructor with one argument
    foo.emplace_back(0);
}
