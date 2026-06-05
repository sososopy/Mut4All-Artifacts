
#include <iostream>

int main()
{
    std::ios_base::seekdir dir = std::ios_base::end;
    switch ( dir )
    {
        case std::ios_base::end:
        case std::ios_base::beg:
        case std::ios_base::cur:
            break;
    }
}

