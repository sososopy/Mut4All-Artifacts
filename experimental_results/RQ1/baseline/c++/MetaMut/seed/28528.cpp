
#include <map>
#include <memory>

using int_map = std::map<int, std::unique_ptr<int>>;

void f(int_map cl);

void f2() {
    int_map cl;
    f(cl);
}
