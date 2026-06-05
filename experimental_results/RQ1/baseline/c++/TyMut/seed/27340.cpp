

#include <iostream>
struct NonConstant{
    NonConstant(){
        std::cout<<"";
    }

    ~NonConstant(){std::cout << "non constant delete" <<std::endl;}
};

struct Constant{

    ~Constant(){std::cout << "constant delete" <<std::endl;}
};

NonConstant a;

Constant b;

int main(){	}
