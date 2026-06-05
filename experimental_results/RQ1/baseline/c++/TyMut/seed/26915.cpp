#include <iostream>
#include <map>
#include <string>
using namespace std;

struct LoadModel;

template<typename T>
class Hello
{
public:
    template<typename TLayerOp, typename...TArgs>
    friend void LayerOperation(TLayerOp*,
                               Hello& p_layer,
                               TArgs&&...args)
    {
        cout << 10 << endl;
    }

    friend void LayerOperation(LoadModel*,
                               Hello& p_layer,
                               int& p)
    {
        cout << 20 << endl;
    }
};


int main()
{
    int p;
    Hello<bool> h;
    LayerOperation(static_cast<LoadModel*>(nullptr), h, p);
}

