
#include <iostream>

int main()
{
    std::cout <<
                []( int x )
                {
                    return [&](int y)
                    {
                        return x+y;
                    }(2);
                }(5)
              << "\n";
    std::cout << []( int x )
                 {
                    return [&x](int y)
                    {
                        return x+y;
                    };
                 }(2)(5)
              << "\n";
    std::cout << []( int x )
                 {
                    return [x](int y)
                    {
                        return x+y;
                    };
                 }(2)(5)
              << "\n";

    return 0;
}
