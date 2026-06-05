

#include <list>
#include <memory>

template <typename T>
class CreateTypes
{
public:
    CreateTypes()
    {
        [] (...)
        {

        }
        (
            (
                m_types.push_back(std::unique_ptr<T>(new T()))
            )
        );
    }

private:
    std::list<std::unique_ptr<int>> m_types;
};

int main()
{
    CreateTypes<int> createTypes;
    return 0;
}

