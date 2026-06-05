
#include <vector>

template<typename T>
class TemplateKlass
{
};

void funktion()
{
    std::vector<TemplateKlass<int> > result;
    char segment;
    result.emplace_back(segment);
}
