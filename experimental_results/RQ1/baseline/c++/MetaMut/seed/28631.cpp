
struct C{
    void* operator new[](std::size_t N) noexcept(false){  // #1
        std::cout<<"abc\n";
        auto p =  malloc(10);
        return p;
    }
};
int n = -1;
auto ptr = new C[n];
std::cout<< ptr;
delete [] ptr;
