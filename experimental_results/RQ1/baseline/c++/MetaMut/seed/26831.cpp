

    #include <map>
    #include <iostream>

    struct X
    {
        int x;
        int y;
        
        bool operator<(const X& other) const { return x < other.x; }
    };

    int main()
    {    
        std::map<X, int> detailsMap = {
            { X{1, 2}, 7 }, 
            { X{3, 4}, 7 },
            { X{5, 6}, 7 }
        };
        
        std::cout << detailsMap.size() << std::endl;
        
        auto f = *(detailsMap.begin());
        
        std::cout << f.first.x << " " << f.first.y << std::endl;
    }


