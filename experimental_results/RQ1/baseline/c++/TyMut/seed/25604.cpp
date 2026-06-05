

#include <iostream>
#include <cstdint>

enum Type : uint32_t { Zero  = 0, One = 1, Two = 2, Mask = 3, Rest = 0xfffffffc };

struct Data {
    explicit constexpr Data(uint32_t data = 0, Type type = Zero) : dataM(data << 2 | type) {}
    constexpr Data(Data const& data, Type type) : dataM(data.dataM | type) {}
    Data(Data const&) = default;

    Type type() const { return Type(dataM & Mask); }
    Data remove_type() const { Data copy(*this); copy.dataM &= Rest; return copy; }
    uint32_t dataM;
};

inline std::ostream& operator<<(std::ostream& stream, Data id) { return stream << id.dataM; }

class Bug
{
public:
    Bug(Data id1, Data id2) : data(id1), first(id2, Two) {}
    Bug(Bug&&) = default;

    Type type() const { return first.type(); }
    Data no_1() const { return first.remove_type(); }
    
    friend std::ostream& operator<<(std::ostream& stream, Bug const& bug)
    {
        switch (bug.type())
        {
        case Zero:
            return stream << "0: " << bug.data.index << " (" << bug.first << ')' << std::endl ;
        case One:
            return stream << "1: " << bug.no_1();
        case Two:
            return stream << "2: " << bug.no_1() << ' ' << bug.data.second;
        default:
            return stream;
        }
    }
private: 
    union U {
        U() {}
        explicit U(unsigned index) : index(index) {}
        explicit U(Data id) : second(id) {}
        unsigned index;
        Data second;
    }; 

    U data;
    Data first;
};

int main() {
    Bug a(Data(1), Data(2));
    std::cout << a << std::endl;
    Bug b(std::move(a));
    std::cout << b << std::endl;
}
