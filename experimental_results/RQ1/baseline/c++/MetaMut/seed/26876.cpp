

#include <iostream>
#include <initializer_list>
#include <string>
#include <vector>
using namespace std;

int main()
{
        int required = 1;
        initializer_list<int> needed = {0, 1, 2};
        needed = {required};
        vector<bool> started = {true};
        for (int item : needed) {
                cout << item <<endl;
        }
        return 0;
}


