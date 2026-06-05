
#include <iostream>
using namespace std;

template<typename ... Pack>
void weeble(Pack ... rest, double x) {
    int y[] = {rest...};
    for (int i = 0; i < sizeof(y)/sizeof(*y); ++i) cout << y[i] << " ";
    cout << x << endl;
}

int main () {
    weeble(123,456,789,3.1416);
}

