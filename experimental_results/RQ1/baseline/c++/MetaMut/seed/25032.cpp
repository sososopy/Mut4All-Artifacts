
#include <locale>

int main()
{
    // all fine
    std::locale loc0(std::locale(), new std::collate_byname<char>(""));
    std::locale loc1 = { std::locale(), new std::collate_byname<char>("") };

    // error: 'std::__cxx11::collate_byname<_CharT>::~collate_byname() [with _CharT = char]' is protected within this context
    std::locale loc2 = std::locale(std::locale(), new std::collate_byname<char>(""));
}
