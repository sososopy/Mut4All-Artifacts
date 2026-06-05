
#ifndef CONSTS_H
    #define CONSTS_H

    namespace Consts
    {
        inline constexpr int Test_v = 120;
    }
#endif



module;


export module Consts;

export namespace Consts
{
    using Consts::Test_v;
}


#include <iostream>
import Consts;

int main()
{
    std::cout << Consts::Test_v;

    return 0;
}


