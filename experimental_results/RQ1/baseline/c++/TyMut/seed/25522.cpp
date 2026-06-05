
#include <functional>

void g(std::function<void () > f)
{}

int main(){
    g([] {} );
}

