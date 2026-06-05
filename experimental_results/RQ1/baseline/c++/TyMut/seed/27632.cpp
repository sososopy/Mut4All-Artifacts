
#include <iostream>

template <typename T, uint32_t CAPACITY>
class VariableSizedArray
{
public:
    void setSize(uint32_t num) { body.numElements = num; }

    T& operator[](uint32_t idx)
    {
        if (idx >= CAPACITY or idx >= body.numElements)
        {
            raiseIndexErr("", 42, "ERROR: Invalid idx: ", idx, 42, body.numElements);
        }
        return body.elems[idx];
    }

    // Changes in parameters make the problem to disappear
    static void raiseIndexErr(const char*, uint32_t, const char* error, uint32_t idx, uint32_t, uint32_t num)
    {
        // If printf is used, problem disappears
        std::cerr << error << idx << "! Capacity: " << CAPACITY << ", number elements: " << num << ::std::endl;
    }

    // private: // If private -- problem disappears
    struct Data
    {
        uint32_t numElements;
        T elems[CAPACITY];
    };
    using Body = Data; 
    Body body; // If Data is used directly here w/o "using", problem disappears
};

static constexpr int outerArraySize = 32;
static constexpr int innerArraySize = 4;

class InnerArrayItem
{
public:
    void use(){};
    uint8_t dummy[1]{}; // when removed or not an array, problem disappears
};

class OuterArrayItem
{
public:
    uint8_t dummy{0}; // When removed, problem disappears
    VariableSizedArray<InnerArrayItem, innerArraySize> innerArray{};
};

struct TestProblem
{
    // When parameter removed (its always 0) -- problem disappears
    static void run(const uint8_t index0)
    {
        VariableSizedArray<OuterArrayItem, outerArraySize> outerArray{};

        outerArray.setSize(outerArraySize);
        auto& outerArrItem = outerArray[index0];
        outerArrItem.innerArray.setSize(innerArraySize);

        // ERROR: invalid idx is reported for outerArray[0].innerArray[0] access!
        outerArrItem.innerArray[index0].use();
    }
};

int main()
{
    TestProblem::run(0);
    std::cout << "Test finished." << std::endl;
    return 0;
}

