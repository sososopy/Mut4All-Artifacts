
#include <iostream>
using namespace std;

int main(){
        int d = 7;
        int c = 7;
        int &b = c;
        b += b++;
        d += d++;
        cout << "c = "<< c << endl;
        cout << "d = "<< d << endl;
return 0;
}

