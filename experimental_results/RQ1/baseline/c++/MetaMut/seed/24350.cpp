
#include <iostream>

struct notpod {
    ~notpod() {}
};

struct noisy {
    ~noisy() { std::cout << "destroy\n"; }
};

struct cont {
    noisy o;
    notpod np;
    int i;
};

int main() {
    {
        notpod && q = cont().np;
        std::cout << "check\n";
    } // cont::~cont here

    {
        int && i = cont().i;
        // cont::~cont here; i is dangling.
        std::cout << "check\n";
    }
}


