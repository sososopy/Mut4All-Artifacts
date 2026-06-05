
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    int test[2][2] ={
        [0]={1,2},
        [1]={3,4},
    };

    for (int x = 0; x<2;x++)
    {
        for (int y = 0; y<2; y++)
        {
            cout << test[x][y] << endl;
        }
    }

    return 0;
}
