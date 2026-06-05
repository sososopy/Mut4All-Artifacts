
#include <string>
template <class T>
std::string func(T a, int b) {
    std::string x;
    int y;
    if (b == 1) {
        x = a;
    } else if (b == 2) {
        y = a;
    }

    return x;
}

int main() {
    std::string c = func<int>(1, 1);   
    return 0;
}
