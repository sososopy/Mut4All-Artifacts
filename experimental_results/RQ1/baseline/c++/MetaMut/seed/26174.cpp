
    #include <vector>
    
    struct Xyz
    {
        Xyz(int, int);
    };
    
    int main() {
        std::vector<Xyz> v;
        v.emplace_back(1); // internal compiler error here
    }
